#include "P528GTest.h"
#include <cctype>
#include <string>
#include <functional>


/*=============================================================================
 |
 |  Description:  The purpose of this is to ensure that CMAKE compiles 
 |                the P.528 model DLL correctly.
 |                Test Data is stored in Data Table format.
 |                Data Table test files are from Recommendation ITU-R P.528-5
 |                publication website:
 |                https://www.itu.int/rec/R-REC-P.528-5-202109-I/en
 |
 *===========================================================================*/
class TestP528DataTables : public ::testing::TestWithParam<std::string>
{
public:
    struct PrintToStringParamName
    {
        template <class ParamType>
        std::string operator() (const testing::TestParamInfo<ParamType>& info) const
        {
            std::string testName = static_cast<std::string>(info.param);
            testName.resize(testName.find_last_of("."));
            testName.erase(remove_if(testName.begin(), testName.end(), [](char c) { return !isalnum(c); }), testName.end());

            return testName;
        }
    };

    double TOLERANCE = 1.0e-1;
    int testStep = 100;
};

/*=============================================================================
 |
 |  Description:  Test case to verify P528 results are correct
 |
 *===========================================================================*/
TEST_P(TestP528DataTables, TestDataTable) {
    std::string fileName = GetParam();
    std::vector<InputsAndResult> testData = ReadP528InputsAndResultFromDataTable(fileName, testStep);
    EXPECT_NE(static_cast<int>(testData.size()), 0);
    std::cout << "TestP528 from Data Table '" << fileName << "', " << testData.size() << " Test instances." << std::endl;
    for (const auto& data : testData) {
        Result result;
        int rtn = P528(data.d__km, data.h_1__meter, data.h_2__meter, data.f__mhz, data.T_pol, data.p, &result);
        EXPECT_EQ(0, rtn);
        if (!isnan(data.expectedResult.A_fs__db)) {
            EXPECT_NEAR(result.A_fs__db, data.expectedResult.A_fs__db, TOLERANCE);
        }
        EXPECT_NEAR(result.A__db, data.expectedResult.A__db, TOLERANCE);
    }
}

// Note: Microsoft.googletest doesn't have INSTANTIATE_TEST_SUITE_P
// Refer to: https://github.com/microsoft/react-native-windows/issues/9791
INSTANTIATE_TEST_CASE_P(
    TestP528, 
    TestP528DataTables, 
    ::testing::ValuesIn(GetDataTablesFileList()));