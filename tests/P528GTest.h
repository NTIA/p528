#pragma once

#include <gtest/gtest.h>

#include "p528.h"

using namespace ITS::ITU::PSeries::p528;

struct InputsAndResult {
    double d__km;
    double h_1__meter;
    double h_2__meter;
    double f__mhz;
    int T_pol;
    double p;
    int expectedReturn;
    Result expectedResult;
};

std::vector<InputsAndResult> ReadP528InputsAndResultFromPoints(const std::string& filename);

std::vector<std::string> GetDataTablesFileList();

std::vector<InputsAndResult> ReadP528InputsAndResultFromDataTable(const std::string& filename, int testStep);

std::vector<std::string> readCSVRow(const std::string& row);

std::vector<std::vector<std::string>> readCSV(std::istream& in);

