###########################################
## BUILD THE LIBRARY
###########################################
proplib_message("Configuring library ${PROJECT_SOURCE_DIR} ${LIB_NAME}")

set(LIB_HEADERS "${PROJECT_SOURCE_DIR}/include")
set(LIB_FILES
    # p528
    "p528/CombineDistributions.cpp"
    "p528/FindKForYpiAt99Percent.cpp"
    "p528/GetPathLoss.cpp"
    "p528/InverseComplementaryCumulativeDistributionFunction.cpp"
    "p528/LineOfSight.cpp"
    "p528/LinearInterpolation.cpp"
    "p528/LongTermVariability.cpp"
    "p528/NakagamiRice.cpp"
    "p528/P528.cpp"
    "p528/RayOptics.cpp"
    "p528/ReflectionCoefficients.cpp"
    "p528/SmoothEarthDiffraction.cpp"
    "p528/TerminalGeometry.cpp"
    "p528/TranshorizonSearch.cpp"
    "p528/Troposcatter.cpp"
    "p528/ValidateInputs.cpp"
    "p528/data.cpp"
    "p528/ReturnCodes.cpp"
    "${LIB_HEADERS}/p528.h"
    #p676
    "p676/GlobalWetPressure.cpp"
    "p676/LineShapeFactor.cpp"
    "p676/NonresonantDebyeAttenuation.cpp"
    "p676/OxygenData.cpp"
    "p676/RayTrace.cpp"
    "p676/RefractiveIndex.cpp"
    "p676/Refractivity.cpp"
    "p676/SlantPath.cpp"
    "p676/SpecificAttenuation.cpp"
    "p676/TerrestrialPath.cpp"
    "p676/WaterVapourData.cpp"
    "p676/WaterVapourDensityToPartialPressure.cpp"
    "${LIB_HEADERS}/p676.h"
    #p835
    "p835/Conversions.cpp"
    "p835/MeanAnnualGlobalReferenceAtmosphere.cpp"
    "${LIB_HEADERS}/p835.h"
)

# By default, create shared library
if (NOT DEFINED BUILD_SHARED_LIBS)
    message(STATUS 
           "STATUS: BUILD_SHARED_LIBS is not defined to build the library: " 
           ${LIB_NAME} ".")
    add_library(${LIB_NAME} SHARED ${LIB_FILES})
else ()
    message(STATUS "STATUS: BUILD_SHARED_LIBS is " ${BUILD_SHARED_LIBS} " to build the library: " ${LIB_NAME} ".")
    add_library(${LIB_NAME} ${LIB_FILES})
endif ()

# Add the include directory
target_include_directories(${LIB_NAME} PUBLIC "${LIB_HEADERS}")
    # "${PROJECT_SOURCE_DIR}/extern/p676/include")

# target_link_libraries(${LIB_NAME} PRIVATE P676)

# Set PropLib compiler option defaults
configure_proplib_target(${LIB_NAME})

# Add definition to get the library name and version inside the library
target_compile_definitions(${LIB_NAME} PRIVATE
    LIBRARY_NAME="${LIB_NAME}"
    LIBRARY_VERSION="${PROJECT_VERSION}"
)

# Platform-specific configurations
if (WIN32)
    set_target_properties(${LIB_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS true)
endif ()
if (UNIX)
    # avoid prefixing "lib" to the output file, for cross-platform consistency
    set_target_properties(${LIB_NAME} PROPERTIES PREFIX "")
endif ()

# Set some target metadata
set_target_properties(
    ${LIB_NAME} PROPERTIES
    OUTPUT_NAME ${LIB_NAME}-${PROJECT_VERSION}-
    SOVERSION ${PROPLIB_SOVERSION}
    VERSION ${PROJECT_VERSION}
    DEBUG_POSTFIX ${ARCH_SUFFIX}
    RELEASE_POSTFIX ${ARCH_SUFFIX}
)

proplib_message("Done configuring library ${LIB_NAME}")
