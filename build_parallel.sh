#!/bin/bash
srcFiles=$(find src/ -name *.c??)

#echo "Building files: $srcFiles"

outName="raytracing"
outDir="bin"
cc="mpic++"
compilerFlags="-Wall"
includeFlags="-Isrc"
linkerFlags=""
defines=""

if !(test -d $outDir); then
    mkdir $outDir
fi

$cc $srcFiles $compilerFlags -o $outDir/$outName $defines $includeFlags $linkerFlags