cd ../../../scripts && ./debug.sh
cd ../docs/usage/c_documentation
rm -rv build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j
