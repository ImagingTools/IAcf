#ifndef iipr_CProjectionData_included
#define iipr_CProjectionData_included


#include "istd/CRange.h"
#include "istd/TOptDelPtr.h"

#include "i2d/CLine2d.h"

#include "iser/ISerializable.h"

#include "iimg/IBitmap.h"
#include "iimg/CGeneralBitmap.h"

#include "iipr/iipr.h"


namespace iipr
{


/**
	Data container representing result of projection processor.
	Internal projection data are stored as bitmap with height equals 1.
	Additionally two offset ranges are stored to describe proportion of real projection to logical projection position.
*/
class CProjectionData: public iser::ISerializable
{
public:
	explicit CProjectionData(
		iimg::IBitmap* dataImagePtr = new iimg::CGeneralBitmap(),
		bool releaseFlag = true);

	/**
		Reset the image projection data.
	*/
	void ResetProjectionData();

	/**
		Get access to bitmap object representing projection data.
		Typically with is equal 1.
	*/
	const iimg::IBitmap& GetProjectionImage() const;
	/**
		Get access to bitmap object representing projection data.
		Typically height is equal 1.
	*/
	iimg::IBitmap& GetProjectionImage();

	/**
		Get projection line.
	*/
	const i2d::CLine2d& GetProjectionLine() const; 

	/**
		Set projection line.
	*/
	void SetProjectionLine(const i2d::CLine2d& projectionLine);

	/**
		Get proportion of logical projection to bitmap represented projection.
		For example, if original projection has 100 logical pixel and in this range is (0.2, 0.8), first bitmap pixel returned by GetProjectionImage
		begins at logical position 20 and the last pixel ends at position 80.
	*/
	const istd::CRange& GetProportionRangeX() const;
	/**
		Set proportion of logical projection to bitmap represented projection.
		\sa GetProportionRangeX
	*/
	void SetProportionRangeX(const istd::CRange& range);

	/**
		Get proportion of logical projection to bitmap represented projection.
		\sa GetProportionRangeX
	*/
	const istd::CRange& GetProportionRangeY() const;
	/**
		Set proportion of logical projection to bitmap represented projection.
		\sa GetProportionRangeX
	*/
	void SetProportionRangeY(const istd::CRange& range);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::TOptDelPtr<iimg::IBitmap> m_projectionImagePtr;
	istd::CRange m_proportionRangeX;
	istd::CRange m_proportionRangeY;
	i2d::CLine2d m_projectionLine;

};


// inline methods

inline const iimg::IBitmap& CProjectionData::GetProjectionImage() const
{
	return *m_projectionImagePtr;
}


inline iimg::IBitmap& CProjectionData::GetProjectionImage()
{
	return *m_projectionImagePtr;
}


inline const i2d::CLine2d& CProjectionData::GetProjectionLine() const
{
	return m_projectionLine;
}


inline void CProjectionData::SetProjectionLine(const i2d::CLine2d& projectionLine)
{
	m_projectionLine = projectionLine;
}


inline const istd::CRange& CProjectionData::GetProportionRangeX() const
{
	return m_proportionRangeX;
}


inline const istd::CRange& CProjectionData::GetProportionRangeY() const
{
	return m_proportionRangeY;
}


} // namespace iipr


#endif // !iipr_CProjectionData_included


