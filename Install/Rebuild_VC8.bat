echo off

echo Building IACF

cd ../Impl/VC8
vcbuild /r IacfAll.sln $All

echo Building ACF done

echo Building ACF Tutorials

cd ../../Docs/Tutorial/Appl/VC8
vcbuild /r TutorialIacf.sln $All
echo Building ACF Tutorials done

cd ../../../../Install

