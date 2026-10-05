# CMake Configuration for P528

This folder contains CMake build configuration and preset for P528. It also includes a P528 Driver CMake configuration for testing the P528 library.

## Prerequisites

 * A modern C/C++ compiler
 * CMake 3.14+ installed (on a Mac, run `brew install cmake`)

## Building and Running ##

### Project Structure

All CMake configuration is inside this cmake folder. 

P528 Propagation library source code is in src folder and header files are in include folder.

### Building Manually

To manually build the project, you can run the following commands step by step. As an example, we use /cmake_build as CMake build folder (cmake binary folder) and /if77install as installation folder.

```
mkdir cmake_build
cd cmake_build
cmake ../cmake
cmake --build .
cmake --install . --config Debug --prefix ../p528install
```

### Cross Platform Building with Preset ###

CMake also supports preset building. The configuration is set in CMake Presets.json. The configurations can be changed based on your preference. The following is an example to build the project in windows x64 debug mode.

```
cmake --preset x64-debug
cmake --build --preset x64-debug
```

### Running the tests ###

```
ctest
```

## Package

```
cpack -G ZIP -C Debug
cpack -G TGZ -C Debug
```
