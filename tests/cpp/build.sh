#!/bin/zsh
cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build_debug
pushd build_debug
make
popd
