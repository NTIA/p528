cmake_minimum_required(VERSION 3.11.0)

project(
  client
  VERSION 0.0.1
  LANGUAGES CXX)
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

file(GLOB SOURCES ./*.cpp)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(. ../../include/ ../../wasm/ ${GTEST_INCLUDE_DIRS})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  link_directories("../../libs/debug")
else()
  link_directories("../../libs/release")
endif()

add_compile_definitions(${ADD_DEFINES} CMAKE_BUILD _CRT_SECURE_NO_DEPRECATE
                        NOMINMAX)

# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests testP528.cpp)
target_link_libraries(runTests PRIVATE nlohmann_json::nlohmann_json
                                       ${GTEST_LIBRARIES} p528)
