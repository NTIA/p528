#include "gtest/gtest.h"

#include "p528.h"


TEST(Examples, TestCase1)
{
    Result res{};

    const auto d__km = 15;
    const auto h_1__meter = 10;
    const auto h_2__meter = 1000;
    const auto f__mhz = 500;
    const int T_pol = 0;
    const auto time_percentage = 50;

    auto ret = P528(d__km, h_1__meter, h_2__meter, f__mhz, T_pol, time_percentage, &res);

    EXPECT_NEAR(res.A__db, 110.0, 0.1);
}

TEST(Examples, TestCase2)
{
    Result res{};

    const auto d__km = 100;
    const auto h_1__meter = 100;
    const auto h_2__meter = 15000;
    const auto f__mhz = 3600;
    const int T_pol = 0;
    const auto time_percentage = 90;

    auto ret = P528(d__km, h_1__meter, h_2__meter, f__mhz, T_pol, time_percentage, &res);

    EXPECT_NEAR(res.A__db, 151.6, 0.1);
}

TEST(Examples, TestCase3)
{
    Result res{};

    const auto d__km = 1500;
    const auto h_1__meter = 15;
    const auto h_2__meter = 10000;
    const auto f__mhz = 5700;
    const int T_pol = 0;
    const auto time_percentage = 10;

    auto ret = P528(d__km, h_1__meter, h_2__meter, f__mhz, T_pol, time_percentage, &res);

    EXPECT_NEAR(res.A__db, 293.4, 0.1);
}

TEST(Examples, TestCase4)
{
    Result res{};

    const auto d__km = 30;
    const auto h_1__meter = 8;
    const auto h_2__meter = 20000;
    const auto f__mhz = 22000;
    const int T_pol = 1;
    const auto time_percentage = 50;

    auto ret = P528(d__km, h_1__meter, h_2__meter, f__mhz, T_pol, time_percentage, &res);

    EXPECT_NEAR(res.A__db, 151.1, 0.1);
}