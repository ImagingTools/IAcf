#ifndef ilolv_IDigitalIo_included
#define ilolv_IDigitalIo_included


#include "istd/IPolymorphic.h"

#include "ilolv/ilolv.h"


namespace ilolv
{


class IDigitalIo: virtual public istd::IPolymorphic
{
public:
	virtual I_DWORD GetInputBits() const = 0;
	virtual void SetOutputBits(I_DWORD value, I_DWORD mask) = 0;
};


} // namespace ilolv


#endif // !ilolv_IDigitalIo_included


