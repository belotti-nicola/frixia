#!/bin/sh

TEST_DIRECTORY=${1}

echo "Test to run: $TEST_DIRECTORY"

sleep 3

mkdir -p ../build
rm -rv ../build/*
cd ../build || exit 1

cmake .. -DCMAKE_BUILD_TYPE=Debug
make && ctest --test-dir /home/nicola/Projects/frixia_project/frixia/build/test/edge_cases
