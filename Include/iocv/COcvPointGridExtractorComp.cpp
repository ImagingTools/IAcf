#include <iocv/COcvPointGridExtractorComp.h>


// OpenCV includes
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

// ACF includes
#include <istd/CChangeGroup.h>
#include <iimg/CBitmap.h>
#include <iimg/CScanlineMask.h>
#include <iprm/TParamsPtr.h>
#include <ilog/CExtMessage.h>
#include <iprm/ISelectionParam.h>
#include <iipr/CPointGridFeature.h>
#include <i2d/CPosition2d.h>
#include <i2d/CPosition2d.h>

// IACF includes
#include <iocv/COcvImage.h>


namespace iocv
{


// reimplemented (iipr::IImageToFeatureProcessor)

int COcvPointGridExtractorComp::DoExtractFeatures(
			const iprm::IParamsSet* paramsPtr,
			const iimg::IBitmap& image,
			iipr::IFeaturesConsumer& results,
			ibase::IProgressManager* /*progressManagerPtr*/)
{
	try{
		results.ResetFeatures();

		iimg::CBitmap aoiBitmap;
		int regionLeft = 0;
		int regionTop = 0;

		i2d::CRectangle aoi;

		if (m_defaultRectangleCompPtr.IsValid()) {
			aoi.CopyFrom(*m_defaultRectangleCompPtr);
		}
		else if (m_aoiRecangleParamIdAttrPtr.IsValid()) {
			iprm::TParamsPtr<i2d::CRectangle> rectParamPtr(paramsPtr, *m_aoiRecangleParamIdAttrPtr);
			if (rectParamPtr.IsValid()) {
				aoi.CopyFrom(*rectParamPtr);
			}
		}

		if (aoi.IsEmpty()) {
			aoiBitmap.CopyFrom(image);
		}
		else {
			iimg::CScanlineMask mask;
			const i2d::CRect inputRect(image.GetBoundingBox());
			mask.CreateFromRectangle(aoi, &inputRect);

			aoiBitmap.CreateBitmap(image.GetPixelFormat(), mask.GetImageSize());

			const i2d::CRect regionRect = mask.GetBoundingBox();

			regionLeft = qMax(regionRect.GetLeft(), 0);
			const int regionRight = qMin(regionRect.GetRight(), inputRect.GetWidth());
			regionTop = qMax(regionRect.GetTop(), 0);
			const int regionBottom = qMin(regionRect.GetBottom(), inputRect.GetHeight());

			const int pixelBits = aoiBitmap.GetPixelBitsCount();
			const int pixelBytes = pixelBits / 8;

			for (int y = regionTop; y < regionBottom; y++) {
				const quint8* inputLinePtr = (quint8*)image.GetLinePtr(y);

				istd::CIntRanges::RangeList rangesList;
				const istd::CIntRanges* rangesPtr = mask.GetPixelRanges(y);
				if (rangesPtr == NULL) {
					continue;
				}

				rangesPtr->GetAsList(inputRect.GetHorizontalRange(), rangesList);

				quint8* outputImageLinePtr = (quint8*)aoiBitmap.GetLinePtr(y - regionTop);

				for (istd::CIntRanges::RangeList::ConstIterator iter = rangesList.begin();
					iter != rangesList.end();
					++iter) {
					const istd::CIntRange& pixelRange = *iter;

					int rangeStart = qMax(pixelRange.GetMinValue(), 0);
					Q_ASSERT(rangeStart >= regionLeft);
					int rangeEnd = qMin(pixelRange.GetMaxValue(), image.GetImageSize().GetX());
					Q_ASSERT(rangeEnd <= regionRight);

					int bytesToCopy = (rangeEnd - rangeStart) * pixelBytes;

					if (bytesToCopy > 0) {
						std::memcpy(outputImageLinePtr + (rangeStart - regionLeft) * pixelBytes,
							inputLinePtr + rangeStart * pixelBytes,
							bytesToCopy);
					}
				}
			}
		}

		cv::Size gridSize(6, 6);
		const imeas::INumericValue* gridValuePtr = NULL; 
		if (m_gridSizeParamIdAttrPtr.IsValid()){
			iprm::TParamsPtr<imeas::INumericValue> gridSizeParamIdAttrPtr(paramsPtr, *m_gridSizeParamIdAttrPtr);
			if (gridSizeParamIdAttrPtr.IsValid()){
				gridValuePtr = gridSizeParamIdAttrPtr.GetPtr();
			}
		}
		else if (m_defaultGridSizeCompPtr.IsValid()){
			gridValuePtr = m_defaultGridSizeCompPtr.GetPtr();
		}

		if (gridValuePtr != NULL){
			imath::CVarVector values = gridValuePtr->GetValues();
			if (values.GetElementsCount() > 1){
				gridSize.width = values.GetElement(0);
				gridSize.height = values.GetElement(1);
			}
		}

		Pattern pattern = PT_CHESSBOARD;
		if (m_patternTypeParamIdAttrPtr.IsValid()){
			iprm::TParamsPtr<iprm::ISelectionParam> paramTypePtr(paramsPtr, *m_patternTypeParamIdAttrPtr);
			if (paramTypePtr.IsValid()){
				int index = paramTypePtr->GetSelectedOptionIndex();
				pattern = index == 0 ? PT_CHESSBOARD : PT_CIRCLES_GRID;
			}
		}
		else if (m_defaultPatternTypeAttrPtr.IsValid()){
			pattern = *m_defaultPatternTypeAttrPtr == 0 ? PT_CHESSBOARD : PT_CIRCLES_GRID;
		}

		cv::Mat view;
		if (!COcvImage::ConvertFromBitmap(aoiBitmap, view)){
			SendCriticalMessage(0, QObject::tr("Can not read input image"));
			return TS_INVALID;
		}

		std::vector<cv::Point2f> pointBuf;

		bool found;
		switch (pattern) // Find feature points on the input format
		{
		case PT_CHESSBOARD:
			found = cv::findChessboardCorners(view, gridSize, pointBuf,
				cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);
			break;
		case PT_CIRCLES_GRID:
			found = findCirclesGrid(view, gridSize, pointBuf);
			break;
		default:
			found = false;
			break;
		}

		if (!found){
			SendErrorMessage(0, QObject::tr("Point grid is not found"));
			return TS_INVALID;
		}

		// improve the found corners' coordinate accuracy for chessboard
		if (pattern == PT_CHESSBOARD){
			cv::Mat& viewGray = view;
			const iimg::IBitmap::PixelFormat pixelFormat = image.GetPixelFormat();

			switch (view.type())
			{
			case CV_8UC4:
			{
				cv::Mat gray;
				cv::cvtColor(view, gray, cv::COLOR_RGBA2GRAY);
				viewGray = gray;
			}
				break;
			case CV_8UC3:
			{
				cv::Mat gray;
				cv::cvtColor(view, gray, cv::COLOR_RGB2GRAY);
				viewGray = gray;
			}
			break;
			default:
				break;
			}

			cornerSubPix(viewGray, pointBuf, cv::Size(11, 11),
				cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::Type::EPS + cv::TermCriteria::Type::MAX_ITER, 30, 0.1));
		}

		iipr::CPointGridFeature* pointGridPtr = new iipr::CPointGridFeature();
		pointGridPtr->SetSize(0, gridSize.width);
		pointGridPtr->SetSize(1, gridSize.height);

		int k = 0;
		for (int j = 0; j < gridSize.height; ++j){
			for (int i = 0; i < gridSize.width; ++i){
				const cv::Point2f& p = pointBuf[k++];
				istd::TIndex<2> ind;
				ind.SetAt(0, i);
				ind.SetAt(1, j);
				pointGridPtr->SetAt(ind, i2d::CVector2d(p.x + regionLeft + 0.5, p.y + regionTop + 0.5));
			}
		}

		if (m_resultConsumerCompPtr.IsValid()){
			ilog::CExtMessage* messagePtr = new ilog::CExtMessage(
				istd::IInformationProvider::IC_INFO,
				0, QObject::tr("Found %1 point(s)").arg(pointBuf.size()), "OpenCV Point Grid Extractor"
			);

			for (const cv::Point2f& p : pointBuf){
				imod::TModelWrap<i2d::CPosition2d>* messagePointPtr = new imod::TModelWrap<i2d::CPosition2d>;
				messagePointPtr->SetPosition(i2d::CVector2d(p.x + regionLeft + 0.5, p.y + regionTop + 0.5));

				messagePtr->InsertAttachedObject(messagePointPtr);
			}

			m_resultConsumerCompPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(messagePtr));
		}

		results.AddFeature(pointGridPtr);

		return TS_OK;
	}
	catch (const cv::Exception& e){
		SendCriticalMessage(0, e.what());

		return TS_INVALID;
	}
	catch (...){
		SendCriticalMessage(0, QObject::tr("Unknown exception"));

		return TS_INVALID;
	}
}


// reimplemented (iproc::IProcessor)

int COcvPointGridExtractorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* inputPtr,
			istd::IChangeable* outputPtr,
			ibase::IProgressManager* progressManagerPtr)
{
	const iimg::IBitmap* inputBitmapPtr = dynamic_cast<const iimg::IBitmap*>(inputPtr);
	if (inputBitmapPtr == NULL){
		SendCriticalMessage(0, QObject::tr("Input image is not defined"));

		return TS_INVALID;
	}

	if (outputPtr == NULL){
		return TS_OK;
	}

	iipr::IFeaturesConsumer* outputConsumerPtr = dynamic_cast<iipr::IFeaturesConsumer*>(outputPtr);
	if (outputConsumerPtr == NULL){
		return TS_INVALID;
	}

	return DoExtractFeatures(paramsPtr, *inputBitmapPtr, *outputConsumerPtr, progressManagerPtr);
}


} // namespace iocv


