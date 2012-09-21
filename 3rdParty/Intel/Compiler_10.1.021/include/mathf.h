/**
*** Copyright (C) 1985-2007 Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
**/

#if defined(__cplusplus)    /* C linkage */
extern "C" {
#endif

#if defined (ceilf)
#undef ceilf
#endif
#if defined (fabsf)
#undef fabsf
#endif
#if defined (floorf)
#undef floorf
#endif
#if defined (fmodf)
#undef fmodf
#endif
#if defined (rintf)
#undef rintf
#endif
#if defined (hypotf)
#undef hypotf
#endif
#if defined (powf)
#undef powf
#endif
#if defined (sqrtf)
#undef sqrtf
#endif
#if defined (expf)
#undef expf
#endif
#if defined (logf)
#undef logf
#endif
#if defined (log10f)
#undef log10f
#endif
#if defined (acosf)
#undef acosf
#endif
#if defined (asinf)
#undef asinf
#endif
#if defined (atanf)
#undef atanf
#endif
#if defined (cosf)
#undef cosf
#endif
#if defined (sinf)
#undef sinf
#endif
#if defined (tanf)
#undef tanf
#endif
#if defined (coshf)
#undef coshf
#endif
#if defined (sinhf)
#undef sinhf
#endif
#if defined (tanhf)
#undef tanhf
#endif

/* Nearest integer, absolute value, etc. */

float  __cdecl ceilf ( float );
float  __cdecl fabsf ( float );
float  __cdecl floorf ( float );
float  __cdecl fmodf ( float , float );
float  __cdecl rintf ( float );
float  __cdecl hypotf (float, float);

/* Power functions */
float  __cdecl powf ( float , float );
float  __cdecl sqrtf ( float );

/* Exponential and logarithmic functions */
float  __cdecl expf ( float );
float  __cdecl logf ( float );
float  __cdecl log10f ( float );

/* Trigonometric functions */

float  __cdecl acosf ( float );
float  __cdecl asinf ( float );
float  __cdecl atanf ( float );
float  __cdecl cosf ( float );
float  __cdecl sinf ( float );
float  __cdecl tanf ( float );

/* Hyperbolic functions */
float  __cdecl coshf ( float );
float  __cdecl sinhf ( float );
float  __cdecl tanhf ( float );

#if defined(__cplusplus)    /* end C linkage */
}
#endif
