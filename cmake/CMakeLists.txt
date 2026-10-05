# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.

cmake_minimum_required (VERSION 3.14)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
endif()

set(LIB_NAME "p528")

# set the project name and version
project (
    "${LIB_NAME}-distribution" 
    VERSION 1.0.0.0
    DESCRIPTION "Recommendation ITU-R P.528-5 - U.S. Reference Implementation Distribution"
    LANGUAGES "CXX"
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}" CACHE STRING "Set the CMAKE Archive Output Directory")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}" CACHE STRING "Set the CMAKE Library Output Directory")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}" CACHE STRING "Set the CMAKE Runtime Output Directory")

# If not specified, fall back to Debug build type
set(DEFAULT_BUILD_TYPE "Debug")

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "STATUS: Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)

    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include(CMakeDependentOption)
include(GNUInstallDirs)

# By Default, build shared libaray
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build using shared libraries" FORCE)
set(RUN_TESTS ON CACHE BOOL "Run C++ unit tests with Google Test" FORCE)
set(ENABLE_INSTALL ON CACHE BOOL "Enable installation of (${LIB_NAME}. (Projects embedding (${LIB_NAME} may want to turn this OFF.)" FORCE)

get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)
set (INCLUDE_DIR ${PARENT_DIR}/include)
message(STATUS "STATUS: ${PROJECT_NAME} PARENT_DIR is " ${PARENT_DIR})

# add the library
add_subdirectory(${LIB_NAME})

if (RUN_TESTS)         # Build and run unit tests
    set(GOOGLETEST_DIR "${PARENT_DIR}/test/lib/googletest")
    if (EXISTS "${GOOGLETEST_DIR}/CMakeLists.txt")
        enable_testing()
        add_subdirectory(test)
    else ()
        message(SEND_ERROR
            "ERROR: Unable to build tests. GoogleTest submodule is missing. "
            "Run `git submodule init && git submodule update` and try again."
        )
    endif()
endif ()

message(STATUS "STATUS: ${PROJECT_NAME} VERSION_MAJOR is " ${${PROJECT_NAME}_VERSION_MAJOR})
message(STATUS "STATUS: ${PROJECT_NAME} VERSION_MINOR is " ${${PROJECT_NAME}_VERSION_MINOR})

if (ENABLE_INSTALL)
    # setup installer
    include(InstallRequiredSystemLibraries)
    set(CPACK_RESOURCE_FILE_LICENSE "${PARENT_DIR}/License.md")
    set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
    set(CPACK_SOURCE_GENERATOR "TGZ" CACHE STRING "Set the CPack Archive Generator")
    include(CPack)
endif ()