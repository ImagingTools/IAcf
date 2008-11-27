if not exist Temp goto DelTempSkip
rmdir /Q /S Temp
:DelTempSkip

if not exist TempExcl goto DelTempExclSkip
rmdir /Q /S TempExcl
:DelTempExclSkip

echo Copying files to Temp directory...
"%ACFDIR%/bin/ReleaseVC8/AcfDist.exe" DistConfig.xml

echo Appending subversion information...
SubWCRev .. TempExcl/iacf.iss TempExcl/iacf-ext.iss

echo Createing installation file...
iscc.exe /Q TempExcl\iacf-ext.iss

echo Installation done
pause
