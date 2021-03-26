# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.15)

project ("ITU-R-P528-4")

set(CMAKE_CXX_STANDARD 11)


include_directories(${PROJECT_SOURCE_DIR}/include)

set(Headers
    include/LOSTable.h
    include/p528.h)

set(Sources
    src/AbsorptionParameters.cpp
    src/CalculateEffectiveRayLength.cpp
    src/CombineDistributions.cpp
    src/data.cpp
    src/FindKForYpiAt99Percent.cpp
    src/GetPathLoss.cpp
    src/InverseComplementaryCumulativeDistributionFunction.cpp
    src/LinearInterpolation.cpp
    src/LineOfSight.cpp
    src/LongTermVariability.cpp
    src/LOSTable.cpp
    src/NakagamiRice.cpp
    src/P528.cpp
    src/P528Ex.cpp
    src/RayOptics.cpp
    src/RayTrace.cpp
    src/ReflectionCoefficients.cpp
    src/SmoothEarthDiffraction.cpp
    src/TerminalGeometry.cpp
    src/TranshorizonAbsorptionLoss.cpp
    src/TranshorizonSearch.cpp
    src/Troposcatter.cpp
    src/ValidateInputs.cpp)

# Add source to this project's executable.
add_library(${PROJECT_NAME} STATIC ${Sources} ${Headers})