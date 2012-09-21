@echo off
Rem Intel(R) C++ Compiler for applications running IA-32

echo.
echo Intel(R) C++ Compiler for applications running IA-32, Version 10.1.021
echo Copyright (C) 1985-2008 Intel Corporation. All rights reserved.
echo.

@call "C:\Program Files\Microsoft Visual Studio 8\VC\Bin\Vcvars32.bat"
title Intel(R) C++ Compiler for applications running IA-32, Version 10.1.021

echo.

SET ICPP_COMPILER10=C:\Program Files\Intel\Compiler\C++\10.1.021

SET INTEL_SHARED=C:\Program Files\Common Files\Intel\Shared Files

SET INTEL_LICENSE_FILE=C:\Program Files\Common Files\Intel\Licenses

SET PATH=%ICPP_COMPILER10%\Ia32\Bin;%INTEL_SHARED%\Ia32\Bin;%PATH%

SET LIB=%ICPP_COMPILER10%\Ia32\Lib;%LIB%

SET INCLUDE=%ICPP_COMPILER10%\Ia32\Include;%INCLUDE%

