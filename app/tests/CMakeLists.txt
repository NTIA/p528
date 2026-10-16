############################################
## CONFIGURE COMMAND LINE DRIVER TESTS
############################################
add_executable(
    ${DRIVER_TEST_NAME}
    "TempTextFile.cpp"
    "TestDriver.cpp"
    "TestDriverP528.cpp"
    "TempTextFile.h"
    "TestDriver.h"
    "${DRIVER_HEADERS}/Driver.h"
)

# Add the include directories
target_include_directories(
    ${DRIVER_TEST_NAME} PUBLIC
    "${DRIVER_HEADERS}"
    "${PROJECT_SOURCE_DIR}/app/tests"
)

# Set PropLib compiler option defaults
configure_proplib_target(${DRIVER_TEST_NAME})

# Make driver executable location available to source
target_compile_definitions(${DRIVER_TEST_NAME} PRIVATE DRIVER_LOCATION="$<TARGET_FILE:${DRIVER_NAME}>")

###########################################
## SET UP AND DISCOVER TESTS
###########################################
target_link_libraries(${DRIVER_TEST_NAME} PRIVATE ${LIB_NAME} GTest::gtest_main)
gtest_discover_tests(${DRIVER_TEST_NAME})
