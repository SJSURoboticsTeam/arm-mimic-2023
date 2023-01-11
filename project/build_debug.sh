#!/bin/bash

# Build everything. Not sure if conan must be ran each time prob not

# clear old build files
rm -r build/*

cd build

conan install .. -s build_type=Debug

cmake .. -D CMAKE_BUILD_TYPE=Debug

make -j

echo "Done building"