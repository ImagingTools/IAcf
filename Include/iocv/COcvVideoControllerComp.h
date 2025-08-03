#ifndef iocv_COcvVideoControllerComp_included
#define iocv_COcvVideoControllerComp_included


// OpenCV includes
#include <opencv2/opencv.hpp>
#undef min
#undef max

// ACF includes
#include <istd/TDelPtr.h>
#include <icomp/CComponentBase.h>
#include <iproc/TSyncProcessorCompBase.h>

// ACF-Solutions includes
#include <icam/IBitmapAcquisition.h>
#include <imm/IVideoController.h>


namespace iocv
{


/**
	Component for video controlling using OpenCV API.
*/
class COcvVideoControllerComp:
			public iproc::TSyncProcessorCompBase<icam::IBitmapAcquisition>,
			virtual public imm::IVideoController
{
public:
	typedef iproc::TSyncProcessorCompBase<icam::IBitmapAcquisition> BaseClass;

	I_BEGIN_COMPONENT(COcvVideoControllerComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(imm::IMediaController);
		I_REGISTER_INTERFACE(imm::IVideoInfo);
		I_REGISTER_INTERFACE(imm::IVideoController);
		I_ASSIGN(m_frameDataCompPtr, "FrameData", "Frame instance will be loaded with loader", true, "FrameData");
	I_END_COMPONENT;

	COcvVideoControllerComp();

	// reimplemented (icam::IBitmapAcquisition)
	virtual istd::CIndex2d GetBitmapSize(const iprm::IParamsSet* paramsPtr) const override;

	// reimplemented (iproc::IProcessor)
	virtual iproc::IProcessor::TaskState DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL) override;

	// reimplemented (imm::IMediaController)
	virtual QString GetOpenedMediumUrl() const override;
	virtual bool OpenMediumUrl(const QString& url, bool autoPlay = true) override;
	virtual void CloseMedium() override;
	virtual bool IsPlaying() const override;
	virtual bool SetPlaying(bool state = true) override;
	virtual double GetMediumLength() const override;
	virtual double GetCurrentPosition() const override;
	virtual bool SetCurrentPosition(double position) override;
	virtual int GetSupportedFeatures() const override;

	// reimplemented (imm::IVideoInfo)
	virtual int GetFramesCount() const override;
	virtual double GetFrameInterval() const override;
	virtual istd::CIndex2d GetFrameSize() const override;
	virtual double GetPixelAspectRatio() const override;

	// reimplemented (imm::IVideoController)
	virtual int GetCurrentFrame() const override;
	virtual bool SetCurrentFrame(int frameIndex) override;

	// reimplemented (ifile::IFileTypeInfo)
	virtual bool GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr = NULL, int flags = -1, bool doAppend = false) const override;
	virtual QString GetTypeDescription(const QString* extensionPtr = NULL) const override;

protected:
	virtual bool GrabCurrentFrame(iimg::IBitmap& result) const;
	virtual bool SeekToPosition(int frameIndex) const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentDestroyed() override;

private:
	void EnsureMediumClosed();
	int GetStreamPosition() const;
	bool MoveToNextFrame() const;

private:
	I_REF(istd::IChangeable, m_frameDataCompPtr);

	QString m_mediumUrl;
	bool m_isPlaying;

	istd::TDelPtr<cv::VideoCapture> m_capturePtr;
	mutable int m_currentFrameIndex;
	int m_framesCount;
};


} // namespace iocv


#endif // !iocv_COcvVideoControllerComp_included

