#pragma once


// ACF includes
#include <imod/TModelWrap.h>
#include <iprm/IOptionsList.h>
#include <iprm/ISelectionParam.h>
#include <iproc/TSyncProcessorCompBase.h>
#include <iedge/IEdgesExtractor.h>

// OpenCV includes
#include <opencv2/opencv.hpp>


namespace iocv
{


class COcvEdgeExtractorComp: public iproc::TSyncProcessorCompBase<iedge::IEdgesExtractor>
{
public:
	typedef iproc::TSyncProcessorCompBase<iedge::IEdgesExtractor> BaseClass;

	I_BEGIN_COMPONENT(COcvEdgeExtractorComp);
		I_REGISTER_SUBELEMENT(ApproxModeList);
		I_REGISTER_SUBELEMENT_INTERFACE(ApproxModeList, iprm::IOptionsList, ExtractApproxModeList);
		I_REGISTER_SUBELEMENT_INTERFACE(ApproxModeList, istd::IChangeable, ExtractApproxModeList);
		I_REGISTER_SUBELEMENT_INTERFACE(ApproxModeList, imod::IModel, ExtractApproxModeList);
		I_ASSIGN(m_aoiParamIdAttrPtr, "AoiParamId", "ID of area of interest in parameter set", false, "AoiParams");
		I_ASSIGN(m_defaultAoiCompPtr, "DefaultAoi", "Area of interest used if not specified in parameters", false, "DefaultAoi");
		I_ASSIGN(m_approxModeParamIdAttrPtr, "ApproxModeParamId", "ID of contour approximation mode parameter in parameter set", false, "ApproxMode");
		I_ASSIGN(m_defaultApproxModeCompPtr, "DefaultApproxMode", "Default contour approximation model used if not specified in parameters", false, "DefaultApproxMode");
	I_END_COMPONENT;

public:
	//	reimplemented (iedge::IEdgesExtractor)
	virtual bool DoContourExtraction(
				const iprm::IParamsSet* paramsPtr,
				const iimg::IBitmap& bitmap,
				iedge::CEdgeLineContainer& result) const;

	// reimplemented (iproc::IProcessor)
	virtual int DoProcessing(
				const iprm::IParamsSet* paramsPtr,
				const istd::IPolymorphic* inputPtr,
				istd::IChangeable* outputPtr,
				ibase::IProgressManager* progressManagerPtr = NULL);

private:
	template <class InterfaceType>
	static InterfaceType* ExtractApproxModeList(COcvEdgeExtractorComp& component)
	{
		return &component.m_approxModeList;
	}

private:
	I_ATTR(QByteArray, m_aoiParamIdAttrPtr);
	I_REF(i2d::IObject2d, m_defaultAoiCompPtr);

	I_ATTR(QByteArray, m_approxModeParamIdAttrPtr);
	I_REF(iprm::ISelectionParam, m_defaultApproxModeCompPtr);

	struct ApproxMode
	{
		ApproxMode(int cvMode = CV_CHAIN_APPROX_NONE, const QString& name = QString(), const QString& description = QString())
		{
			this->cvMode = cvMode;
			this->name = name;
			this->description = description;
		}

		int cvMode;
		QString name;
		QString description;
	};

	typedef QVector<ApproxMode> ApproxModes;


	class ApproxModeList: virtual public iprm::IOptionsList
	{
	public:
		ApproxModeList();

		int GetOpenCvMode(int index) const;

		// reimplemented (iprm::IOptionsList)
		virtual int GetOptionsFlags() const;
		virtual int GetOptionsCount() const;
		virtual QString GetOptionName(int index) const;
		virtual QString GetOptionDescription(int index) const;
		virtual QByteArray GetOptionId(int index) const;
		virtual bool IsOptionEnabled(int index) const;

	private:
		ApproxModes m_supportedApproxModes;
	};

	imod::TModelWrap<ApproxModeList> m_approxModeList;
};


} // namespace iocv


