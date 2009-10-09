#ifndef iocv_COcvVideoControllerComp_included
#define iocv_COcvVideoControllerComp_included


// OpenCV includes
#include "cxtypes.h"
#include "cv.h"
#include "highgui.h"


// ACF includes
#include "icomp/CComponentBase.h"

#include "imm/IVideoController.h"


namespace iocv
{


/**
	Component for video controlling using OpenCV API.
*/
class COcvVideoControllerComp:
			public icomp::CComponentBase,
			virtual public imm::IVideoController
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(COcvVideoControllerComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(imm::IMediaController);
		I_REGISTER_INTERFACE(imm::IVideoInfo);
		I_REGISTER_INTERFACE(imm::IVideoController);
		I_ASSIGN(m_frameDataCompPtr, "FrameData", "Frame instance will be loaded with loader", true, "FrameData");
	I_END_COMPONENT();

	COcvVideoControllerComp();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentDestroyed();

	// reimplemented (imm::IMediaController)
	virtual istd::CString GetOpenedMediumUrl() const;
	virtual bool OpenMediumUrl(const istd::CString& url, bool autoPlay = true);
	virtual void CloseMedium();
	virtual bool IsPlaying() const;
	virtual bool SetPlaying(bool state = true);
	virtual double GetMediumLength() const;
	virtual double GetCurrentPosition() const;
	virtual bool SetCurrentPosition(double position);
	virtual int GetSupportedFeatures() const;

	// reimplemented (imm::IVideoInfo)
	virtual int GetFramesCount() const;
	virtual double GetFrameIntervall() const;
	virtual istd::CIndex2d GetFrameSize() const;
	virtual double GetPixelAspectRatio() const;

	// reimplemented (imm::IVideoController)
	virtual int GetCurrentFrame() const;
	virtual bool SetCurrentFrame(int frameIndex);
	virtual bool GrabFrame(iimg::IBitmap& result, int frameIndex = -1) const;

protected:
	virtual bool GrabCurrentFrame(iimg::IBitmap& result) const;
	virtual bool SeekToPostion(int frameIndex) const;

private:
	void EnsureMediumClosed();

private:
	I_REF(istd::IChangeable, m_frameDataCompPtr);

	istd::CString m_mediumUrl;
	bool m_isPlaying;

	istd::TPointer<CvCapture> m_capturePtr;
};


} // namespace iocv


#endif // !iocv_COcvVideoControllerComp_included

