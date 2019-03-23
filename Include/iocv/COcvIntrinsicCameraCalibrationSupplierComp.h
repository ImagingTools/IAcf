#pragma once


// ACF includes
#include <iinsp/TSupplierCompWrap.h>
#include <iimg/IBitmapProvider.h>
#include <iipr/IImageToFeatureProcessor.h>


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
		I_REGISTER_INTERFACE(i2d::ICalibrationProvider);
		I_ASSIGN(m_bitmapProviderCompPtr, "BitmapProvider", "Provide input image", true, "BitmapProvider");
		I_ASSIGN(m_pointGridExtractorCompPtr, "PointGridExtractor", "Extractor of the point grid on image", true, "PointGridExtractor");
		I_ASSIGN(m_cellSizeParamId, "CellSizeParamsId", "ID of point grid cell size in parameter set (type imeas::INumericValue): cell size in logical units", false, "CellSizeParam");
		I_ASSIGN(m_defaultCellSizeAttrPtr, "DefaultCellSize", "Default size of single cell (in logical units ) if no checkboard parameters specified", true, 10);
		I_END_COMPONENT;

	// reimplemented (i2d::ICalibrationProvider)
	virtual const i2d::ICalibration2d* GetCalibration() const override;

protected:
	// reimplemented (iinsp::TSupplierCompWrap)
	virtual int ProduceObject(ProductType& result) const override;

private:
	I_REF(iimg::IBitmapProvider, m_bitmapProviderCompPtr);
	I_REF(iipr::IImageToFeatureProcessor, m_pointGridExtractorCompPtr);
	I_ATTR(QByteArray, m_cellSizeParamId);
	I_ATTR(double, m_defaultCellSizeAttrPtr);
};


} // namespace iocv


