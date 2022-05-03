@ECHO OFF
SETLOCAL EnableDelayedExpansion

SET srcFiles=
FOR /r %%f in (*.cpp) DO (
    SET srcFiles=!srcFiles! %%f
)

ECHO "Building files:" %srcFiles%

SET outName=raytracing
SET outDir=bin
SET cc=clang++
SET compilerFlags=-Wall
SET includeFlags=-Isrc
SET linkerFlags=
SET defines=

%cc% %srcFiles% %compilerFlags% -o %outDir%\%outName%.exe %defines% %includeFlags% %linkerFlags%