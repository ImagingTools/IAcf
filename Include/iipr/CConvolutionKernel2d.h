#ifndef iipr_CConvolutionKernel2d_included
#define iipr_CConvolutionKernel2d_included


// ACF includes
#include "ibase/CSize.h"


// IACF includes
#include "iipr/iipr.h"


namespace iipr
{

/**
	Implementation of a general 2D convolution kernel. 
	Internally the kernel is represented by an array of filter weights and offset positions
	for fast access to the corresponding bitmap values.

	For access and iterating through the kernel values you can use the Begin() and End() function, 
	which returns an iterator/accessor.
*/
class CConvolutionKernel2d
{
public:
	struct KernelValue
	{
		int pixelOffset;
		double kernelWeight;
	};

	typedef std::vector<KernelValue> KernelValues;
	typedef typename KernelValues::const_iterator Iterator;

	CConvolutionKernel2d();
	CConvolutionKernel2d(const imath::ISampledFunction2d& function);

	/**
		Return \c true if the kernel was succesfully initialized for a bitmap.
	*/
	bool IsValid() const;

	/**
		Create kernel offsets with respect to image size.
	*/
	void InitForBitmap(const iimg::IBitmap& bitmap) const;

	Iterator Begin() const;
	Iterator End() const;

	double GetWeightsSum() const;
	i2d::CRectangle GetBoundingBox() const;

protected:
	KernelValues m_values;
	ibase::CSize m_kernelSize;

	double m_sumWeights;
};


bool CConvolutionKernel2d::IsValid() const
{
	return !m_values.empty();
}

void CConvolutionKernel2d::InitForBitmap(const iimg::IBitmap& bitmap) const
{
	I_ASSERT(!m_kernelSize.IsNull());
	if (m_kernelSize.IsNull()){
		return;
	}

	int linesDifference = bitmap.GetLinesDifference();

	int halfWidth = m_kernelSize.GetX() / 2;
	int halfHeight = m_kernelSize.GetY() / 2;

	I_ASSERT((m_kernelSize.GetX() * m_kernelSize.GetY()) == int(m_values.size()));

	typename KernelValues::const_iterator elementIter = m_values.begin(); 

	for (int y = -halfHeight; y <= halfHeight; y++){
		for (int x = -halfWidth; x <= halfWidth; x++){
			KernelValue& kernelValue = const_cast<KernelValue&>(*elementIter);

			kernelValue.first = x + y * linesDifference;

			++elementIter;
		}
	}
}


inline typename CConvolutionKernel2d::Iterator CConvolutionKernel2d::Begin() const
{
	return m_values.begin();
}


inline typename CConvolutionKernel2d::Iterator CConvolutionKernel2d::End() const
{
	return m_values.end();
}


inline double CConvolutionKernel2d::GetWeightsSum() const
{
	return m_sumWeights;
}


inline i2d::CRectangle CConvolutionKernel2d::GetBoundingBox() const
{
	return i2d::CRectangle(m_kernelSize);
}


} // namespace iipr


#endif // !iipr_CConvolutionKernel2d_included

