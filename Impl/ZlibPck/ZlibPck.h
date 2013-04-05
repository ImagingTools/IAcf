#ifndef ZlibPck_included
#define ZlibPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "ifile/TFileSerializerComp.h"

#include "izlib/CGzXmlReadArchive.h"
#include "izlib/CGzXmlWriteArchive.h"


/**
	Package besed on external ZLIB library.
*/
namespace ZlibPck
{


typedef ifile::TFileSerializerComp<izlib::CGzXmlReadArchive, izlib::CGzXmlWriteArchive> GzXmlFileSerializer;


} // namespace ZlibPck


#endif // !ZlibPck_included


