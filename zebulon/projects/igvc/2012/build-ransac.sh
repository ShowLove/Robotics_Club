#/bin/bash

NUM_CORES=$(cat /proc/cpuinfo | grep processor | wc -l)

cd ext/ransac
mkdir build
cd build
mkdir debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug . ../../
make -j$NUM_CORES
cd ../
mkdir release
cd release
cmake . ../..
make -j$NUM_CORES
gksudo make install
cd ../debug
gksudo make install
