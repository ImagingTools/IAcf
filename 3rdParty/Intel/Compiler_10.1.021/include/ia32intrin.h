/**
*** Copyright (C) 1985-2008 Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
**/

#ifndef _IA32INTRIN_H_INCLUDED
#define _IA32INTRIN_H_INCLUDED

#include <wmmintrin.h>

#if defined __cplusplus
extern "C" { /* Begin "C" */
  /* Intrinsics use C name-mangling. */
#endif /* __cplusplus */

/* Implement RDTSC and RDPMC */
extern __int64 __cdecl _rdtsc(void);
extern __int64 __cdecl _rdpmc(int);

/* Implements BSWAP */
extern int __cdecl _bswap(int);

/* Implement BSR and BSF */
extern int __cdecl _bit_scan_forward(int);
extern int __cdecl _bit_scan_reverse(int);

/* Implements a 32-bit popcount */
extern int __cdecl _popcnt32(int);

/* Implements short rotates */
extern unsigned short __cdecl _rotwl(unsigned short, int);
extern unsigned short __cdecl _rotwr(unsigned short, int);

/* Implements integer rotates */
extern unsigned int __cdecl _rotl(unsigned int, int);
extern unsigned int __cdecl _rotr(unsigned int, int);

/* Implements long rotates */
extern unsigned long __cdecl _lrotl(unsigned long, int);
extern unsigned long __cdecl _lrotr(unsigned long, int);

#if defined __cplusplus
}; /* End "C" */
#endif /* __cplusplus */

#endif /* _IA32INTRIN_H_INCLUDED */
