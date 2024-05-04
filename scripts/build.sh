#!/usr/bin/sh

rm -rv  ../build/*
cd ../build
cmake .. -DCMAKE_BUILD_TYPE=Release
./frixia
