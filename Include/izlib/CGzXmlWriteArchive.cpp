#include "CGzXmlWriteArchive.h"


namespace izlib
{


CGzXmlWriteArchive::CGzXmlWriteArchive(
			const QString& filePath,
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader,
			const iser::CArchiveTag& rootTag)
:	BaseClass(versionInfoPtr, rootTag),
	BaseClass2(filePath)
{
	m_file = gzopen(filePath.toLocal8Bit(), "wb");

	if (m_file != NULL){
		WriteXmlHeader();
	
		if (serializeHeader){
			SerializeAcfHeader();
		}
	}
}


CGzXmlWriteArchive::~CGzXmlWriteArchive()
{
	if (m_file != NULL){
		WriteXmlFooter();

		gzclose(m_file);
	}
}


// protected methods

// reimplemented (iser::CXmlWriteArchiveBase)

bool CGzXmlWriteArchive::WriteString(const QByteArray& value)
{
	if (!value.isEmpty()){
		return (m_file != NULL) && gzwrite(m_file, value, int(value.size())) != 0;
	}
	else{
		return true;
	}
}


} // namespace izlib


