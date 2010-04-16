#ifndef iorn_included
#define iorn_included


#include "istd/istd.h"
#include "icomp/icomp.h"


/**
	Library containing core of concept of Witold Gantzkes 'objectron'.
	Objectron is something like layer in neural network, it has similiar behaviuor on interface level.
	It means objectron has input vector and output. It can calculate the output from input.
	It can be leaned self-associative or based on teached learning - it depends on implementation.
	The main difference to ANN is that objectron defines no assumption about internal realisation of this interface.
	It means classic algebraic realisation can be used, some recursive combinations of slave objectrons etc.
*/
namespace iorn
{


} // namespace iorn


#endif // !iorn_included

