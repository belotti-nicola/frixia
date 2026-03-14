#!/bin/sh

# Se viene passato un argomento lo usa come build type,
# altrimenti usa Release come default
BUILD_TYPE=${1:-Release}

cd ../build || exit 1
ctest -N
