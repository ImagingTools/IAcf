echo off

if not exist Temp goto DelTempSkip
echo Removing old temporary files...
rmdir /Q /S Temp
:DelTempSkip

if not exist TempExcl goto DelTempExclSkip
echo Removing old temporary scripts...
rmdir /Q /S TempExcl
:DelTempExclSkip

mkdir Temp
mkdir TempExcl

set COMPILER_EXT=VC9

rem call Rebuild.bat

rem call CopyDlls.bat

call Create_TechnicalDoc.bat

call Create_VC_Commercial.bat

rem call Compile_Setup.bat

rem echo Installation done

rem pause

rem echo Removing temporary files...
rem rmdir /Q /S Temp
rem rmdir /Q /S TempExcl

