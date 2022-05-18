@ECHO OFF
SETLOCAL EnableDelayedExpansion

SET srcFiles=
FOR /r %%f in (*.c??) DO (
    SET srcFiles=!srcFiles! %%f
)

REM ECHO "Building files:" %srcFiles%

SET outName=raytracing
SET outDir=bin
SET cc=clang++
SET compilerFlags=-Wall -std=c++17
SET includeFlags=-Isrc
SET linkerFlags=
SET defines=

IF NOT EXIST %outDir% MKDIR %outDir%
%cc% %srcFiles% %compilerFlags% -o %outDir%\%outName%.exe %defines% %includeFlags% %linkerFlags%