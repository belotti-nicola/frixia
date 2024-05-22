#!/usr/bin/sh

mkdir -p ../build
rm -rv  ../build/*
cd ../build
cmake .. -DCMAKE_BUILD_TYPE=Debug ..
make -j 3
cd ../build && gdb frixia
