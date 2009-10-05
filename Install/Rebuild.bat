echo off

echo Building IACF

cd ..\Impl\%COMPILER_EXT%
vcbuild /r IacfAll.sln $All

echo Building ACF done

cd ..\..\Install

rem echo Building IACF Tutorials

rem cd ..\..\Docs\Tutorial\Appl\%COMPILER_EXT%
rem vcbuild /r TutorialIacf.sln $All
rem echo Building IACF Tutorials done

rem cd ..\..\..\..\Install

