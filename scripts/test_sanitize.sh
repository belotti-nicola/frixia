#!/bin/sh

# Se viene passato un argomento lo usa come build type,
# altrimenti usa Release come default
BUILD_TYPE=${1:-Release}

echo "Build type: $BUILD_TYPE"

mkdir -p ../build
rm -rv ../build/*
cd ../build || exit 1

cmake -DCMAKE_C_FLAGS="-g -O0 -fsanitize=address -fno-omit-frame-pointer" \
      -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address" \
      ..
make  && ctest -VV --output-on-failure
