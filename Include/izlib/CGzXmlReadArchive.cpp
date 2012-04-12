#include "CGzXmlReadArchive.h"


namespace izlib
{


CGzXmlReadArchive::CGzXmlReadArchive(const QString& filePath, bool serializeHeader, const iser::CArchiveTag& rootTag)
:	BaseClass(rootTag),
	BaseClass2(filePath),
	m_useLastReadChar(false)
{
	m_file = gzopen(filePath.toLocal8Bit(), "rb");

	if (m_file != NULL){
		ReadXmlHeader();
	
		if (serializeHeader){
			SerializeAcfHeader();
		}
	}
}


CGzXmlReadArchive::~CGzXmlReadArchive()
{
	if (m_file != NULL){
		ReadXmlFooter();

		gzclose(m_file);
	}
}


// reimplemented (iser::CXmlReadArchiveBase)

bool CGzXmlReadArchive::ReadToDelimeter(
			const QByteArray& delimeters,
			QByteArray& result,
			bool skipDelimeter,
			char* foundDelimeterPtr)
{
	if (m_file == NULL){
		return false;
	}

	int cutFromPos = -2;
	int cutToPos = -2;

	QByteArray readString;

	if (!m_useLastReadChar){
		m_lastReadChar = gzgetc(m_file);
	}

	while (m_lastReadChar >= 0){
		int foundPosition = delimeters.indexOf(char(m_lastReadChar));
		if (foundPosition >= 0){
			m_useLastReadChar = !skipDelimeter;

			if (cutFromPos < 0){
				if (cutToPos < 0){
					result = "";

					return true;
				}

				cutFromPos = 0;
			}

			if (cutToPos < 0){
				cutToPos = int(readString.size());
			}

			result = readString.mid(cutFromPos, cutToPos - cutFromPos);

			if (foundDelimeterPtr != NULL){
				*foundDelimeterPtr = delimeters.at(foundPosition);
			}

			return true;
		}

		readString += char(m_lastReadChar);

		if (!isspace((unsigned char)m_lastReadChar) && !iscntrl((unsigned char)m_lastReadChar)){
			cutToPos = int(readString.size());

			if (cutFromPos < 0){
				cutFromPos = cutToPos - 1;
			}
		}

		m_lastReadChar = gzgetc(m_file);
	}

	return false;
}


} // namespace izlib


