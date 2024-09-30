cmake_minimum_required(VERSION 3.14)

if (NOT DEFINED LIB_NAME)
    set(LIB_NAME "p528")
endif()
message(STATUS "STATUS: Set LIB_NAME: " ${LIB_NAME})

project (
    "${LIB_NAME}"
    VERSION 5.1.0.0
    DESCRIPTION "Recommendation ITU-R P.528-5 - U.S. Reference Implementation"
    LANGUAGES "CXX"
)

set (LIB_SOURCE "${PARENT_DIR}/src/")
set (LIB_HEADER "${INCLUDE_DIR}/")

set(LIB_FILES
    ${LIB_HEADER}p528.h
    ${LIB_HEADER}p676.h
    ${LIB_HEADER}p835.h
    ${LIB_SOURCE}p528/CombineDistributions.cpp
    ${LIB_SOURCE}p528/data.cpp
    ${LIB_SOURCE}p528/FindKForYpiAt99Percent.cpp
    ${LIB_SOURCE}p528/GetPathLoss.cpp
    ${LIB_SOURCE}p528/InverseComplementaryCumulativeDistributionFunction.cpp
    ${LIB_SOURCE}p528/LinearInterpolation.cpp
    ${LIB_SOURCE}p528/LineOfSight.cpp
    ${LIB_SOURCE}p528/LongTermVariability.cpp
    ${LIB_SOURCE}p528/NakagamiRice.cpp
    ${LIB_SOURCE}p528/P528.cpp
    ${LIB_SOURCE}p528/RayOptics.cpp
    ${LIB_SOURCE}p528/ReflectionCoefficients.cpp
    ${LIB_SOURCE}p528/SmoothEarthDiffraction.cpp
    ${LIB_SOURCE}p528/TerminalGeometry.cpp
    ${LIB_SOURCE}p528/TranshorizonSearch.cpp
    ${LIB_SOURCE}p528/Troposcatter.cpp
    ${LIB_SOURCE}p528/ValidateInputs.cpp
    ${LIB_SOURCE}p676/GlobalWetPressure.cpp
    ${LIB_SOURCE}p676/LineShapeFactor.cpp
    ${LIB_SOURCE}p676/NonresonantDebyeAttenuation.cpp
    ${LIB_SOURCE}p676/OxygenData.cpp
    ${LIB_SOURCE}p676/RayTrace.cpp
    ${LIB_SOURCE}p676/RefractiveIndex.cpp
    ${LIB_SOURCE}p676/Refractivity.cpp
    ${LIB_SOURCE}p676/SlantPath.cpp
    ${LIB_SOURCE}p676/SpecificAttenuation.cpp
    ${LIB_SOURCE}p676/TerrestrialPath.cpp
    ${LIB_SOURCE}p676/WaterVapourData.cpp
    ${LIB_SOURCE}p676/WaterVapourDensityToPartialPressure.cpp
    ${LIB_SOURCE}p835/Conversions.cpp
    ${LIB_SOURCE}p835/MeanAnnualGlobalReferenceAtmosphere.cpp
    )

# Create shared library
if(NOT DEFINED BUILD_SHARED_LIBS)
    message(STATUS "STATUS: BUILD_SHARED_LIBS is not defined to build " ${PROJECT_NAME} " library.")
    add_library(${PROJECT_NAME} SHARED ${LIB_FILES})
else()
    message(STATUS "STATUS: BUILD_SHARED_LIBS is " ${BUILD_SHARED_LIBS} " to build " ${PROJECT_NAME} " library.")
    add_library(${PROJECT_NAME} ${LIB_FILES})
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(${PROJECT_NAME}
                           PUBLIC
                           ${INCLUDE_DIR}
                          )

if (DEFINED ENABLE_INSTALL AND ENABLE_INSTALL)

    # Create a variable called installable_libs that is a list of all
    # libraries we want to install (e.g. MathFunctions and tutorial_compiler_flags)
    # Then install the installable libraries to the lib folder.
    set(installable_libs ${PROJECT_NAME})

    install(TARGETS ${installable_libs}
            DESTINATION ${CMAKE_INSTALL_BINDIR})

    install(TARGETS ${installable_libs}
            DESTINATION ${CMAKE_INSTALL_LIBDIR})

    # Install the library headers to the include folder.
    # Hint: Use the FILES and DESTINATION parameters

    install(FILES 
            ${LIB_HEADER}p528.h 
            ${LIB_HEADER}p676.h 
            ${LIB_HEADER}p835.h 
            DESTINATION 
            ${CMAKE_INSTALL_INCLUDEDIR}
    )

endif()
