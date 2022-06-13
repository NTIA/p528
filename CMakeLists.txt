# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.15)

project ("ITU-R-P528-5")

set(CMAKE_CXX_STANDARD 11)

enable_testing()

# Include sub-projects.
add_subdirectory ("Tests")

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )

if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )

if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later.

include_directories(${PROJECT_SOURCE_DIR}/include)

set(Headers
    include/p528.h
    include/p676.h
    include/p835.h)

set(Sources
    src/p528/CombineDistributions.cpp                               
    src/p528/data.cpp                                               
    src/p528/FindKForYpiAt99Percent.cpp                             
    src/p528/GetPathLoss.cpp                                        
    src/p528/InverseComplementaryCumulativeDistributionFunction.cpp 
    src/p528/LinearInterpolation.cpp                                
    src/p528/LineOfSight.cpp                                        
    src/p528/LongTermVariability.cpp                                
    src/p528/NakagamiRice.cpp                                       
    src/p528/P528.cpp                                               
    src/p528/RayOptics.cpp                                          
    src/p528/ReflectionCoefficients.cpp                             
    src/p528/SmoothEarthDiffraction.cpp                             
    src/p528/TerminalGeometry.cpp                                   
    src/p528/TranshorizonSearch.cpp                                 
    src/p528/Troposcatter.cpp                                       
    src/p528/ValidateInputs.cpp                                     
    src/p676/GlobalWetPressure.cpp                  
    src/p676/LineShapeFactor.cpp                    
    src/p676/NonresonantDebyeAttenuation.cpp        
    src/p676/OxygenData.cpp                         
    src/p676/RayTrace.cpp                           
    src/p676/RefractiveIndex.cpp                    
    src/p676/Refractivity.cpp                       
    src/p676/SlantPath.cpp                          
    src/p676/SpecificAttenuation.cpp                
    src/p676/TerrestrialPath.cpp                    
    src/p676/WaterVapourData.cpp                    
    src/p676/WaterVapourDensityToPartialPressure.cpp
    src/p835/Conversions.cpp                        
    src/p835/MeanAnnualGlobalReferenceAtmosphere.cpp)

# Add source to this project's executable.
add_library(${PROJECT_NAME} STATIC ${Sources} ${Headers})

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
