#!/bin/sh

# Se viene passato un argomento lo usa come build type,
# altrimenti usa Release come default
BUILD_TYPE=${1:-Release}

echo "Build type: $BUILD_TYPE"

mkdir -p ../build
rm -rv ../build/*
cd ../build || exit 1

cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE
make && ctest --output-on-failure
