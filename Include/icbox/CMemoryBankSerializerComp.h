#ifndef icbox_CMemoryBankSerializerComp_included
#define icbox_CMemoryBankSerializerComp_included


#include <iser/IVersionInfo.h>
#include <ifile/IFilePersistence.h>

#include <icomp/CComponentBase.h>

#include <ilog/TLoggerCompWrap.h>


namespace icbox
{


class CMemoryBankSerializerComp:
			public ilog::CLoggerComponentBase,
			virtual public ifile::IFilePersistence
{
public:
	typedef ilog::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_CBIOS_ERROR = 0xad20
	};

	I_BEGIN_COMPONENT(CMemoryBankSerializerComp);
		I_REGISTER_INTERFACE(ifile::IFilePersistence);

		I_ASSIGN(m_versionInfoCompPtr, "VersionInfo", "Provide information about archive versions", false, "VersionInfo");
		I_ASSIGN(m_memoryBankIdAttrPtr, "MemoryBank", "Memory bank of Crypto Box", true, 1);
		I_ASSIGN(m_partitionIdAttrPtr, "PartitionId", "Partition ID will be used", true, 999);
		I_ASSIGN(m_accessKeyAttrPtr, "AccessKey", "User or administrator key", false, "demo");
		I_ASSIGN(m_isAdminKeyAttrPtr, "IsAdminKey", "True, if access key is admin key", true, false);
	I_END_COMPONENT;

	CMemoryBankSerializerComp();

	// reimplemented (ifile::IFilePersistence)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const;
	virtual int LoadFromFile(
				istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;
	virtual int SaveToFile(
				const istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const;

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr = NULL, int flags = -1, bool doAppend = false) const;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const;

protected:
	bool CheckError(quint32 errorCode) const;
	bool EnsurePartitionOpened() const;
	bool EnsurePartitionClosed() const;
	bool ReadFromMem(int offset, void* bufferPtr, int size) const;
	bool WriteToMem(int offset, const void* bufferPtr, int size) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(iser::IVersionInfo, m_versionInfoCompPtr);
	I_ATTR(int, m_memoryBankIdAttrPtr);
	I_ATTR(int, m_partitionIdAttrPtr);
	I_ATTR(QString, m_accessKeyAttrPtr);
	I_ATTR(bool, m_isAdminKeyAttrPtr);

	bool m_isInitialized;
	mutable bool m_isOpened;
	mutable bool m_isLogged;
};


} // namespace icbox


#endif // !icbox_CMemoryBankSerializerComp_included


