#!/bin/zsh

rebuild=1
# rebuild=0

usedebug=1
# usedebug=0

## debug
if [[ ${usedebug} > 0 ]]; then
    cmake_options=-DUSE_DEBUG=ON
    buildfolder=build_debug_lib
else
    cmake_options=-DUSE_DEBUG=OFF
    buildfolder=build_release_lib
fi

em_image=emscripten/emsdk:3.1.11
em_name=emscripten_3_1_11_lib
# em_image=emscripten/emsdk:2.0.0
# em_name=emscripten_2_0_0_lib

## docker
em_container=$(docker ps -f name="\^${em_name}\$" -q)
if [ -z "${em_container}" ]; then
    echo "emscripten not running. starting...";
    em_container=$(docker ps -a -f name=${em_name} -q)
    if [ -z "${em_container}" ]; then
        docker run --network host -d --name ${em_name} -v $(pwd)/..:/src -u $(id -u):$(id -g) ${em_image} sleep infinity
    else
        docker start ${em_name}
    fi
else
    echo "emscripten already running. '${em_container}'";
fi

## force rebuild
if [[ ${rebuild} > 0 ]]; then
    echo "deleting ${buildfolder}"
    rm -rf ./${buildfolder}
fi

if [ -d ${buildfolder} ]; then
    echo ${buildfolder} folder already exists
    docker exec -it --privileged -u $(id -u):$(id -g) ${em_name} sh -c "cd /src/wasm \
    && cd ${buildfolder} \
    && make -j 8"
else
    mkdir ${buildfolder}
    docker exec -it --privileged -u $(id -u):$(id -g) ${em_name} sh -c "cd /src/wasm \
    && cd ${buildfolder} \
    && emcmake cmake ../../linux/ $cmake_options \
    && make -j 8"
fi
