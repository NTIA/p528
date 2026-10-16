/** @file P528.cpp
 * P.528 driver input parsing tests
 */
#include "TestDriver.h"

#include <string>  // for std::string

/*******************************************************************************
 * Driver test fixture for the P.528 Model
 ******************************************************************************/
class ModelTest: public DriverTest {
    protected:
        void SetUp() override {
            DriverTest::SetUp();
            model_params.out_file = params.out_file;
        }
        void TestModel(const std::string &inputs, const int expected_rtn) {
            int model_rtn = RunDriverWithInputFile(inputs, model_params);
            EXPECT_EQ(model_rtn, expected_rtn);
        }

        std::string ModelInputs;   ///< String to hold input file contents
        DrvrParams model_params;   ///< Default command line arguments
};

TEST_F(ModelTest, TestSuccess) {
    ModelInputs
        = "d__km,100\nh_1__meter,1000\nh_2__meter,10000\n"
          "f__mhz,1000\nT_pol,0\np,50";
    TestModel(ModelInputs, SUCCESS);
}

TEST_F(ModelTest, TestParseError) {
    ModelInputs = "unknown_param,0.0";
    TestModel(ModelInputs, DRVRERR__PARSE);
}

TEST_F(ModelTest, TestParseDistanceError) {
    ModelInputs = "d__km,invalid";
    TestModel(ModelInputs, DRVRERR__PARSE_DIST);
}

TEST_F(ModelTest, TestParseLowHeightError) {
    ModelInputs = "h_1__meter,invalid";
    TestModel(ModelInputs, DRVRERR__PARSE_H1);
}

TEST_F(ModelTest, TestParseHighHeightError) {
    ModelInputs = "h_2__meter,invalid";
    TestModel(ModelInputs, DRVRERR__PARSE_H2);
}

TEST_F(ModelTest, TestParseFrequencyError) {
    ModelInputs = "f__mhz,invalid";
    TestModel(ModelInputs, DRVRERR__PARSE_FREQ);
}

TEST_F(ModelTest, TestParsePolarizationError) {
    ModelInputs = "T_pol,invalid";
    TestModel(ModelInputs, DRVRERR__PARSE_POL);
}

TEST_F(ModelTest, TestParseInvalidPolarizationValue) {
    ModelInputs = "T_pol,2";
    TestModel(ModelInputs, DRVRERR__PARSE_POL);
}

TEST_F(ModelTest, TestParseTimePercentageError) {
    ModelInputs = "p,invalid";
    TestModel(ModelInputs, DRVRERR__PARSE_PTime);
}
