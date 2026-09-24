#include "P528GTest.h"


/*=============================================================================
 |
 |  Description:  The purpose of this is to ensure that CMAKE compiles
 |                the P.528 model DLL correctly.
 |                Test Data is stored in CSV format.
 |
 *===========================================================================*/
class TestP528 : public ::testing::Test {
protected:
    void SetUp() override {
        // Load test data from CSV
        testData = ReadP528InputsAndResultFromPoints("P528Test_001.csv");
    }

    // Vector to hold test data
    std::vector<InputsAndResult> testData;

    double TOLERANCE = 1.0e-10;
};

/*=============================================================================
 |
 |  Description:  Test case to verify P528 results are correct
 |
 *===========================================================================*/
TEST_F(TestP528, TestP528Success) {
    // Ensure test data was loaded
    EXPECT_NE(static_cast<int>(testData.size()), 0);
    std::cout << "TestP528 from 'P528Test_001.csv': " << testData.size() << " Test instances." << std::endl;
    for (const auto& data : testData) {
        Result result;
        int rtn = P528(data.d__km, data.h_1__meter, data.h_2__meter, data.f__mhz, data.T_pol, data.p, &result);

        EXPECT_EQ(rtn, data.expectedReturn);
        EXPECT_EQ(result.propagation_mode, data.expectedResult.propagation_mode);
        EXPECT_EQ(result.warnings, data.expectedResult.warnings);
        EXPECT_NEAR(result.A__db, data.expectedResult.A__db, TOLERANCE);
        EXPECT_NEAR(result.A_fs__db, data.expectedResult.A_fs__db, TOLERANCE);
        EXPECT_NEAR(result.A_a__db, data.expectedResult.A_a__db, TOLERANCE);
        EXPECT_NEAR(result.theta_h1__rad, data.expectedResult.theta_h1__rad, TOLERANCE);
    }
}
