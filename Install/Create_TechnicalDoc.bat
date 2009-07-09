echo Creation of technical documentation...

echo Create doxyfile...
"%ACFDIR%/bin/DebugVC8/Acf.exe" Create_Doxyfile.arx -config ../Config/IacfCore.xpc

cd TempExcl

doxygen

cd ..

echo Technical documentation created
