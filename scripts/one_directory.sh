#!/bin/sh

PREFIX=test
TEST_DIRECTORY=$PREFIX/$1


if [[ ! -e "$TEST_DIRECTORY" ]]; then
    echo "$TEST_DIRECTORY does not exist"
    exit -1
elif [[ ! -d "$1" ]]; then
    echo "$TEST_DIRECTORY exists, but is not a directory"
    exit -1
fi

echo "Test to run: $TEST_DIRECTORY"

sleep 1 && echo "\t-3"
sleep 1 && echo "\t-2"
sleep 1 && echo "\t-1"


mkdir -p ../build
rm -rv ../build/*
cd ../build || exit 1

cmake .. -DCMAKE_BUILD_TYPE=Debug
make && ctest --test-dir $TEST_DIRECTORY
