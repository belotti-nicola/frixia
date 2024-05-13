#!/usr/bin/sh

rm -rv  ../build/*
cmake .. -DCMAKE_BUILD_TYPE=Release -S ../ -D ../build
../build/frixia
