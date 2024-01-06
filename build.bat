@echo off

mkdir build
pushd build
clang-cl /Od /arch:AVX2 /EHcs /std:c17 /Zi ..\source\*.cpp User32.lib
popd ..
