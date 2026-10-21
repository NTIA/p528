/** @file TestDriver.cpp
 * General tests for the driver executable
 */
#include "TestDriver.h"

#include <string>  // for std::string

TEST_F(DriverTest, MissingOptionError1) {
    // Test case: missing option between two provided flags
    std::string cmd = executable + " -i -o out.txt";
    SuppressOutputs(cmd);
    int rtn = RunCommand(cmd);
    EXPECT_EQ(DRVRERR__MISSING_OPTION, rtn);
}

TEST_F(DriverTest, MissingOptionError2) {
    // Test case: missing option at the end of command
    std::string cmd = executable + " -i";
    SuppressOutputs(cmd);
    int rtn = RunCommand(cmd);
    EXPECT_EQ(DRVRERR__MISSING_OPTION, rtn);
}

TEST_F(DriverTest, InvalidOptionError) {
    std::string cmd = executable + " -X";
    SuppressOutputs(cmd);
    int rtn = RunCommand(cmd);
    EXPECT_EQ(DRVRERR__INVALID_OPTION, rtn);
}

TEST_F(DriverTest, OpeningInputFileError) {
    params.in_file = "/invalid/path/input.xyz";
    int rtn = RunDriver(params);
    EXPECT_EQ(DRVRERR__OPENING_INPUT_FILE, rtn);
}

TEST_F(DriverTest, OpeningOutputFileError) {
    // Provide valid inputs but invalid output file path
    std::string inputs
                = "d__km,100\nh_1__meter,1000\nh_2__meter,10000\n"
          "f__mhz,1000\nT_pol,0\np,50";
    params.out_file = "/invalid/path/output.xyz";
    int rtn = RunDriverWithInputFile(inputs, params);
    EXPECT_EQ(DRVRERR__OPENING_OUTPUT_FILE, rtn);
}

TEST_F(DriverTest, ValidationInFileError) {
    std::string cmd = executable + " -o out.txt";
    SuppressOutputs(cmd);
    int rtn = RunCommand(cmd);
    EXPECT_EQ(DRVRERR__VALIDATION_IN_FILE, rtn);
}

TEST_F(DriverTest, ValidationOutFileError) {
    // Input file does not need to exist here, just has to be specified
    std::string cmd = executable + " -i in.txt";
    SuppressOutputs(cmd);
    int rtn = RunCommand(cmd);
    EXPECT_EQ(DRVRERR__VALIDATION_OUT_FILE, rtn);
}
