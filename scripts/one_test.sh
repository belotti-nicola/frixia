#!/bin/sh

TEST=${1}

echo "Test to run: $TEST"

sleep 3

mkdir -p ../build
rm -rv ../build/*
cd ../build || exit 1

cmake .. -DCMAKE_BUILD_TYPE=Debug
make && ctest -R $TEST
