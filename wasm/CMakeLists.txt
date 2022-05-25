cmake_minimum_required(VERSION 3.11.0)

project(p528json VERSION 1.0.0)

include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.9.1)
FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

message(STATUS "EMSCRIPTEN_VERSION: " ${EMSCRIPTEN_VERSION})
add_definitions(-std=gnu++17)
if(USE_DEBUG)
  add_definitions(-DDEBUG -gsource-map -O0 -fdeclspec)
  set(BUILD_OUTPUT_DIRECTORY "debug")
else()
  add_definitions(-O3 -s DISABLE_EXCEPTION_CATCHING=1 -fdeclspec)
  set(BUILD_OUTPUT_DIRECTORY "release")
endif()

include_directories(. ../include)

file(GLOB SOURCES ./*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/dist")
add_executable(p528json ${SOURCES})

add_compile_definitions(${ADD_DEFINES} CMAKE_BUILD _CRT_SECURE_NO_DEPRECATE
                        NOMINMAX)

target_link_libraries(p528json PRIVATE nlohmann_json::nlohmann_json p528)

set(CMAKE_EXECUTABLE_SUFFIX ".js")

if(USE_DEBUG)
  message(STATUS "Setting compilation target to debug WASM")
  set_target_properties(
    p528json
    PROPERTIES
      LINK_FLAGS
      "-L../../libs/debug/wasm \
       -s EXPORTED_RUNTIME_METHODS='[ccall]'"
  )
else()
  message(STATUS "Setting compilation target to release WASM")
  set_target_properties(
    p528json
    PROPERTIES
      LINK_FLAGS
      "-L../../libs/release/wasm \
       -s EXPORTED_RUNTIME_METHODS='[ccall]'"
  )
endif()
