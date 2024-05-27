#!/usr/bin/sh

mkdir -p ../build
rm -rv  ../build/*
cd ../build
cmake .. -DCMAKE_BUILD_TYPE=Release ..
cd ../build && make && make test