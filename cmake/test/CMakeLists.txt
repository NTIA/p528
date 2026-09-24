cmake_minimum_required(VERSION 3.14)

if (NOT DEFINED LIB_NAME)
    set(LIB_NAME "p528")
endif()
message(STATUS "STATUS: Set LIB_NAME: " ${LIB_NAME})

project (
    "${LIB_NAME}GTest"
     VERSION 5.1.0.0
    DESCRIPTION "Recommendation ITU-R P.528-5 - U.S. Reference Implementation Google Tests"
    LANGUAGES "CXX"
)

set(INSTALL_GTEST ON CACHE BOOL "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.)" FORCE)

set (TEST_SOURCE "${PARENT_DIR}/test/src/")

set(TEST_FILES
    ${TEST_SOURCE}P528GTest.h
    ${TEST_SOURCE}P528GTestUtils.cpp
    ${TEST_SOURCE}TestP528.cpp
    ${TEST_SOURCE}TestP528DataTables.cpp
)

include_directories(${INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${TEST_FILES})

###########################################
## SET UP AND DISCOVER TESTS
###########################################
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

if (DEFINED BUILD_SHARED_LIBS AND BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS_OLD ${BUILD_SHARED_LIBS})
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build using shared libraries" FORCE)
    message(STATUS "STATUS: BUILD_SHARED_LIBS is " ${BUILD_SHARED_LIBS} " to build gtest library.")
endif()
add_subdirectory(${GOOGLETEST_DIR} "lib/googletest")

include(GoogleTest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} ${LIB_NAME} GTest::gtest_main)
gtest_discover_tests(${PROJECT_NAME})

if (DEFINED BUILD_SHARED_LIBS_OLD)
    set(BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS_OLD} CACHE BOOL "Build using shared libraries" FORCE)
    message(STATUS "STATUS: BUILD_SHARED_LIBS is " ${BUILD_SHARED_LIBS} ", old: " ${BUILD_SHARED_LIBS_OLD})
endif()

if (DEFINED ENABLE_INSTALL AND ENABLE_INSTALL)
    if (DEFINED INSTALL_GTEST AND INSTALL_GTEST)
        install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
        install(TARGETS gtest DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif()