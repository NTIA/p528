cmake_minimum_required(VERSION 3.8.0)
project(p528 VERSION 5.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(
    ../include
    .
)

if(EMSCRIPTEN)
    message(STATUS "EMSCRIPTEN_VERSION: " ${EMSCRIPTEN_VERSION})
    add_definitions(-std=gnu++14)
    if (USE_DEBUG)
        add_definitions(-DDEBUG -g4 -O0 -fdeclspec)
        set(BUILD_OUTPUT_DIRECTORY "debug/wasm")
    else()
        add_definitions(-O3 -s DISABLE_EXCEPTION_CATCHING=1)
        set(BUILD_OUTPUT_DIRECTORY "release/wasm")
    endif()
else()
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_OUTPUT_DIRECTORY "debug")
  else()
    set(BUILD_OUTPUT_DIRECTORY "release")
  endif()
endif ()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -Wno-deprecated")
SET(CMAKE_CXX_FLAGS_DEBUG "-g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,-zdefs")
link_libraries(dl)

add_compile_definitions(
    ${ADD_DEFINES}
    CMAKE_BUILD
  	_CRT_SECURE_NO_DEPRECATE
    NOMINMAX
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}../../../libs/${BUILD_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}../../../libs/${BUILD_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}../../../libs/${BUILD_OUTPUT_DIRECTORY})

set_target_properties(${target} PROPERTIES ENABLE_EXPORTS 1)

set(MYLINKTYPE)
if(EMSCRIPTEN)
    set(MYLINKTYPE STATIC)
else()
    set(MYLINKTYPE SHARED)
endif()

add_library(${PROJECT_NAME} ${MYLINKTYPE}
    ../src/p528/CombineDistributions.cpp
    ../src/p528/data.cpp
    ../src/p528/FindKForYpiAt99Percent.cpp
    ../src/p528/GetPathLoss.cpp
    ../src/p528/InverseComplementaryCumulativeDistributionFunction.cpp
    ../src/p528/LinearInterpolation.cpp
    ../src/p528/LineOfSight.cpp
    ../src/p528/LongTermVariability.cpp
    ../src/p528/NakagamiRice.cpp
    ../src/p528/P528.cpp
    ../src/p528/RayOptics.cpp
    ../src/p528/ReflectionCoefficients.cpp
    ../src/p528/SmoothEarthDiffraction.cpp
    ../src/p528/TerminalGeometry.cpp
    ../src/p528/TranshorizonSearch.cpp
    ../src/p528/Troposcatter.cpp
    ../src/p528/ValidateInputs.cpp
    ../src/p676/GlobalWetPressure.cpp
    ../src/p676/LineShapeFactor.cpp
    ../src/p676/NonresonantDebyeAttenuation.cpp
    ../src/p676/OxygenData.cpp
    ../src/p676/RayTrace.cpp
    ../src/p676/RefractiveIndex.cpp
    ../src/p676/Refractivity.cpp
    ../src/p676/SlantPath.cpp
    ../src/p676/SpecificAttenuation.cpp
    ../src/p676/TerrestrialPath.cpp
    ../src/p676/WaterVapourData.cpp
    ../src/p676/WaterVapourDensityToPartialPressure.cpp
    ../src/p835/Conversions.cpp
    ../src/p835/MeanAnnualGlobalReferenceAtmosphere.cpp
)

target_link_libraries(${PROJECT_NAME}  
    ${XLIBS}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)