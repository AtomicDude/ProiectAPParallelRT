#!/bin/bash
srcFiles=$(find src/ -name *.c??)

#echo "Building files: $srcFiles"

outName="raytracing"
outDir="bin"
cc="mpic++"
compilerFlags="-Wall -fdiagnostics-color=always"
includeFlags="-Isrc"
linkerFlags=""
defines=""

if !(test -d $outDir); then
    mkdir $outDir
fi

output="$( ( $cc $srcFiles $compilerFlags -o $outDir/$outName $defines $includeFlags $linkerFlags; ) 2>&1 )"

bash "custom_clear.sh"
GREEN='\033[1;32m'
BLUE='\033[1;34m'
RESET='\033[0m'
currentPath=$(pwd)
currentCommand=$(basename "$0")
#echo -e "${GREEN}$USER@$HOSTNAME${RESET}:${BLUE}$currentPath${RESET}$ ./$currentCommand"
if !( [ -z "$output" ] ); then
    echo "$output"
fi