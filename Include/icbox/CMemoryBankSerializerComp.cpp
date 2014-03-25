#include "icbox/CMemoryBankSerializerComp.h"


// Windows API
#include <windows.h>

// CryptoBox API
#include "cbios.h"

#include "iser/CMemoryReadArchive.h"
#include "iser/CMemoryWriteArchive.h"


namespace icbox
{


CMemoryBankSerializerComp::CMemoryBankSerializerComp()
:	m_isInitialized(false),
	m_isOpened(false),
	m_isLogged(false)
{
}


// reimplemented (ifile::IFilePersistence)

bool CMemoryBankSerializerComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const QString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iser::ISerializable*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, "Object is not serializable");
		}

		return false;
	}

	if ((flags & QF_ANONYMOUS) == 0){
		return false;
	}

	if (		(*m_memoryBankIdAttrPtr != 3) &&
				!m_accessKeyAttrPtr.IsValid()){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, "User or administrator password is needed to access bank 1 and 2");
		}

		return false;
	}

	if (		((flags & QF_SAVE) != 0) &&
				(*m_memoryBankIdAttrPtr == 2) &&
				(!m_accessKeyAttrPtr.IsValid() || !*m_isAdminKeyAttrPtr)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, "Administrator password is needed to write bank 2");
		}

		return false;
	}

	return m_isOpened && ((filePathPtr == NULL) || filePathPtr->isEmpty());
}


int CMemoryBankSerializerComp::LoadFromFile(
			istd::IChangeable& data,
			const QString& /*filePath*/,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (EnsurePartitionOpened() && IsOperationSupported(&data, NULL, QF_LOAD | QF_ANONYMOUS, false)){
		iser::ISerializable* serializablePtr = CompCastPtr<iser::ISerializable>(&data);
		Q_ASSERT(serializablePtr != NULL);	// it was checked in IsOperationSupported

		quint32 blockSize;
		if (!ReadFromMem(0, &blockSize, sizeof(blockSize)) || (blockSize < 0) || (blockSize > 0xffff)){
			return OS_FAILED;
		}

		QVector<quint8> buffer(blockSize, 0);
		if (ReadFromMem(sizeof(blockSize), &buffer[0], blockSize) || (blockSize > 0xffff)){
			iser::CMemoryReadArchive archive(&buffer, blockSize);

			if (serializablePtr->Serialize(archive)){
				return OS_OK;
			}
			else{
				SendErrorMessage(MI_CANNOT_LOAD, "Cannot load data from Crypto Box, data corrupted");
			}

		}
	}

	return OS_FAILED;
}


int CMemoryBankSerializerComp::SaveToFile(
			const istd::IChangeable& data,
			const QString& /*filePath*/,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (EnsurePartitionOpened() && IsOperationSupported(&data, NULL, QF_SAVE | QF_ANONYMOUS, false)){
		iser::ISerializable* serializablePtr = CompCastPtr<iser::ISerializable>(const_cast<istd::IChangeable*>(&data));
		Q_ASSERT(serializablePtr != NULL);	// it was checked in IsOperationSupported

		iser::CMemoryWriteArchive archive(m_versionInfoCompPtr.GetPtr());
		if (!serializablePtr->Serialize(archive)){
			SendErrorMessage(MI_CANNOT_LOAD, "Cannot store data to archive");
			return OS_FAILED;
		}

		quint32 blockSize = quint32(archive.GetBufferSize());
		if (WriteToMem(0, &blockSize, sizeof(blockSize)) && WriteToMem(sizeof(blockSize), archive.GetBuffer(), blockSize)){
			return OS_OK;
		}
		else{
			SendErrorMessage(MI_CANNOT_LOAD, "Cannot write data to Crypto Box");
		}
	}

	return OS_FAILED;
}


// reimplemented (ifile::IFileTypeInfo)

bool CMemoryBankSerializerComp::GetFileExtensions(QStringList& /*result*/, int /*flags*/, bool /*doAppend*/) const
{
	return false;
}


QString CMemoryBankSerializerComp::GetTypeDescription(const QString* /*extensionPtr*/) const
{
	return QString();
}


// protected methods

bool CMemoryBankSerializerComp::CheckError(quint32 errorCode) const
{
	if (errorCode == 0){
		return true;
	}

	if (errorCode & CBIOS_WARNING_MASK){
		SendWarningMessage(MI_CBIOS_ERROR, QString("CBIOS Warning code ") + QString().setNum(errorCode));

		return true;
	}

	SendErrorMessage(MI_CBIOS_ERROR, QString("CBIOS Error code ") + QString().setNum(errorCode));

	return false;
}


bool CMemoryBankSerializerComp::EnsurePartitionOpened() const
{
	if (m_isOpened){
		return true;
	}

	if (!m_isInitialized){
		return false;
	}

	if (CheckError(::CBIOS_OpenByApp(quint16(*m_partitionIdAttrPtr)))){
		return false;
	}

	m_isOpened = true;

	if (!m_accessKeyAttrPtr.IsValid()){
		return false;
	}

	if (*m_memoryBankIdAttrPtr != 3){
		quint8 password[16] = {0};
		const QByteArray& passwordStr = (*m_accessKeyAttrPtr).toLocal8Bit();
		std::memcpy(password, passwordStr.data(), qMin(int(sizeof(password)), passwordStr.size()));

		if (CheckError((*m_isAdminKeyAttrPtr)? ::CBIOS_APWLogin(password): ::CBIOS_UPWLogin(password))){
			return false;
		}

		m_isLogged = true;
	}

	return true;
}


bool CMemoryBankSerializerComp::EnsurePartitionClosed() const
{
	if (m_isLogged){
		::CBIOS_Logout();

		m_isLogged = false;
	}

	if (m_isOpened){
		::CBIOS_Close();

		m_isOpened = false;
	}

	return true;
}


bool CMemoryBankSerializerComp::ReadFromMem(int offset, void* bufferPtr, int size) const
{
	quint8 password[16] = {0};
	if (*m_memoryBankIdAttrPtr == 3){
		return !CheckError(::CBIOS_ReadRAM3(offset, size, bufferPtr, password));
	}
	else{
		const QByteArray& passwordStr = (*m_accessKeyAttrPtr).toLocal8Bit();
		std::memcpy(password, passwordStr.data(), qMin(int(sizeof(password)), passwordStr.size()));

		if (*m_memoryBankIdAttrPtr == 1){
			return !CheckError(::CBIOS_ReadRAM1(offset, size, bufferPtr, password));
		}
		else if (*m_memoryBankIdAttrPtr == 2){
			return !CheckError(::CBIOS_ReadRAM2(offset, size, bufferPtr, password));
		}
	}

	return false;
}


bool CMemoryBankSerializerComp::WriteToMem(int offset, const void* bufferPtr, int size) const
{
	quint8 password[16] = {0};
	if (*m_memoryBankIdAttrPtr == 3){
		return !CheckError(::CBIOS_WriteRAM3(offset, size, (PVOID)bufferPtr, password));
	}
	else{
		const QByteArray& passwordStr = (*m_accessKeyAttrPtr).toLocal8Bit();
		std::memcpy(password, passwordStr.data(), qMin(int(sizeof(password)), passwordStr.size()));

		if (*m_memoryBankIdAttrPtr == 1){
			return !CheckError(::CBIOS_WriteRAM1(offset, size, (PVOID)bufferPtr, password));
		}
		else if (*m_memoryBankIdAttrPtr == 2){
			return !CheckError(::CBIOS_WriteRAM2(offset, size, (PVOID)bufferPtr, password));
		}
	}

	return false;
}


// reimplemented (icomp::CComponentBase)

void CMemoryBankSerializerComp::OnComponentCreated()
{
	m_isInitialized = (::CBIOS_StartupDLL() != CBIOS_ERR_NOT_INITIALIZED);
}


void CMemoryBankSerializerComp::OnComponentDestroyed()
{
	EnsurePartitionClosed();

	if (m_isInitialized){
		::CBIOS_FinishDLL();

		m_isInitialized = false;
	}
}


} // namespace icbox


