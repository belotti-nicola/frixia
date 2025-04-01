#!/bin/sh

mkdir -p ../build
rm -rv  ../build/*
cd ../build
cmake .. -DCMAKE_BUILD_TYPE=Release ..
if [ $? -ne 0 ]; then
    exit 
fi
make -j 3
if [ $? -ne 0 ]; then
    exit
fi
cd ../build && ./frixia
