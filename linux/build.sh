#!/bin/bash

function buildlib {
  if [ ! -d "build_debug" ]; then
    mkdir build_debug
    cd build_debug
    cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" ..
    cd ..
    cmake -H. -Bbuild_debug
  fi
  if [ ! -d "build_release" ]; then
    mkdir build_release
    cd build_release
    cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ..
    cd ..
    cmake -H. -Bbuild_release
  fi

  cd build_debug
  echo building debug
  cmake --build . 2>outputdebug.txt
  cd ..
  cd build_release

  echo building release
  cmake --build . 2>outputrelease.txt
}
echo $PWD
rm -rfd ../libs
rm -rfd build_*
buildlib