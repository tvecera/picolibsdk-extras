#!/bin/bash

echo "Detecting number of CPU cores..."
PLATFORM=$(uname -s)
echo "$PLATFORM"

if [[ "$PLATFORM" == "Linux" ]]; then
    NUM_CORES=$(nproc --all)
elif [[ "$PLATFORM" == "Darwin" ]]; then
    NUM_CORES=$(sysctl -n hw.ncpu)
else
    echo "Default to 1 if unknown system..."
    NUM_CORES=1
fi

NUM_CORES=$((NUM_CORES / 2))

echo "Use ${NUM_CORES} cores ..."

echo "Clean..."
make clean

echo "Compilation..."
make --jobs=${NUM_CORES} all

echo
cat ./*.siz
echo
