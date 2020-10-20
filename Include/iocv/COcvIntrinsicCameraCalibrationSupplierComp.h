#pragma once


// ACF includes
#include <iinsp/TSupplierCompWrap.h>
#include <iimg/IBitmapProvider.h>
#include <iipr/IImageToFeatureProcessor.h>
#include <icalib/CAffineCalibration2d.h>


// IACF includes
#include <iocv/COcvIntrinsicCameraCalibration.h>


namespace iocv
{


class COcvIntrinsicCameraCalibrationSupplierComp:
			public iinsp::TSupplierCompWrap<COcvIntrinsicCameraCalibration>,
			virtual public i2d::ICalibrationProvider
{
public:
	typedef iinsp::TSupplierCompWrap<COcvIntrinsicCameraCalibration> BaseClass;
	
	I_BEGIN_COMPONENT(COcvIntrinsicCameraCalibrationSupplierComp);
		I_REGISTER_SUBELEMENT(ResultCalibration);
		I_REGISTER_SUBELEMENT_INTERFACE(ResultCalibration, i2d::ICalibration2d, ExtractCalibration);
		I_REGISTER_SUBELEMENT_INTERFACE(ResultCalibration, iser::ISerializable, ExtractCalibration);
		I_REGISTER_INTERFACE(i2d::ICalibration2d);
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapProvider", "Provide input image", true, "BitmapProvider");
		I_ASSIGN(m_pointGridExtractorCompPtr, "PointGridExtractor", "Extractor of the point grid on image", true, "PointGridExtractor");
		I_ASSIGN(m_cellSizeParamId, "CellSizeParamsId", "ID of point grid cell size in parameter set (type imeas::INumericValue): cell size in logical units", false, "CellSize");
		I_ASSIGN(m_defaultCellSizeAttrPtr, "DefaultCellSize", "Default size of single cell (in logical units ) if no checkboard parameters specified", true, 10);
		I_ASSIGN(m_fixK2AttrPtr, "FixK2", "K2 radial distortion coefficient is not changed during the optimization", false, false);
		I_ASSIGN(m_fixK3AttrPtr, "FixK3", "K3 radial distortion coefficient is not changed during the optimization", false, false);
		I_ASSIGN(m_fixPrincipalPointAttrPtr, "FixPrincipalPoint", "The principal point is not changed during the global optimization", false, false);
	I_END_COMPONENT;

	COcvIntrinsicCameraCalibrationSupplierComp();

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const override;

protected:
	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const override;

	template <class InteraceType>
	static InteraceType* ExtractCalibration(COcvIntrinsicCameraCalibrationSupplierComp& parent)
	{
		return static_cast<InteraceType*>(&parent.m_lastCalibration); // parent.m_productPtr.IsValid() ? parent.m_productPtr.GetPtr() : static_cast<InteraceType*>(&parent.m_emptyCalibration);
	}

private:
	I_REF(iimg::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(iipr::IImageToFeatureProcessor, m_pointGridExtractorCompPtr);
	I_ATTR(QByteArray, m_cellSizeParamId);
	I_ATTR(double, m_defaultCellSizeAttrPtr);
	I_ATTR(bool, m_fixK2AttrPtr);
	I_ATTR(bool, m_fixK3AttrPtr);
	I_ATTR(bool, m_fixPrincipalPointAttrPtr);

private:
	icalib::CAffineCalibration2d m_emptyCalibration;
	mutable iocv::COcvIntrinsicCameraCalibration m_lastCalibration;
};


} // namespace iocv


