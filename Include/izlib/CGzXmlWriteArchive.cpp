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
	m_file = gzopen(filePath.toStdString().c_str(), "wb");

	SerializeXmlHeader();

	if (serializeHeader){
		SerializeAcfHeader();
	}
}


CGzXmlWriteArchive::~CGzXmlWriteArchive()
{
	Flush();

	if (m_file != NULL){
		gzclose(m_file);
	}
}


// protected methods

// reimplemented (iser::CXmlWriteArchiveBase)

bool CGzXmlWriteArchive::WriteString(const std::string& value)
{
	if (!value.empty()){
		return (m_file != NULL) && gzwrite(m_file, value.c_str(), int(value.size())) != 0;
	}
	else{
		return true;
	}
}


} // namespace izlib


