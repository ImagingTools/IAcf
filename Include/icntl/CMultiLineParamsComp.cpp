#include "icntl/CMultiLineParamsComp.h"


#include "icntl/CLineParamsComp.h"


namespace icntl
{


// reimplemented (icntl::IMultiLineParams)

int CMultiLineParamsComp::GetLinesCount() const
{
	return m_linesCompPtr.GetCount();
}


ILineParams& CMultiLineParamsComp::GetLineParams(int lineIndex) const
{
	ILineParams* lineParamsPtr = m_linesCompPtr[lineIndex];
	if (lineParamsPtr != NULL){
		return *lineParamsPtr;
	}

	static CLineParamsComp dummyLine;

	return dummyLine;
}


// reimplemented (iser::ISerializable)

bool CMultiLineParamsComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && SerializeName(archive);

	static iser::CArchiveTag linesTag("Lines", "List of line parameter");
	static iser::CArchiveTag lineTag("Line", "Single light barrier position");

	int barriersCount = GetLinesCount();
	retVal = retVal && archive.BeginMultiTag(linesTag, lineTag, barriersCount);
	if (barriersCount != GetLinesCount()){
		return false;
	}

	for (int lineIndex = 0; lineIndex < barriersCount; ++lineIndex){
		retVal = retVal && archive.BeginTag(lineTag);
		ILineParams& lineParams = GetLineParams(lineIndex);
		retVal = retVal && lineParams.Serialize(archive);
		retVal = retVal && archive.EndTag(lineTag);
	}

	retVal = retVal && archive.EndTag(linesTag);

	return retVal;
}


// reimplemented (icomp::IComponent)

void CMultiLineParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetName(*m_nameAttrPtr);
}


} // namespace icntl


