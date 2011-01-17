echo off

echo Copying files to Temp directory...
"%ACFDIR%/bin/Debug%COMPILER_EXT%/Acf.exe" Create_%COMPILER_EXT%_Commercial.arx -config ../Config/Core.xpc

