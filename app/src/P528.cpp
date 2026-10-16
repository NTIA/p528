/** @file P528.cpp
 * Implements driver functions for running the Model.
 */
#include "Driver.h"

#include <fstream>   // for std::ifstream, std::ofstream
#include <iostream>  // for std::cerr
#include <istream>   // for std::istream
#include <ostream>   // for std::endl
#include <string>    // for std::string
#include <tuple>     // for std::tie
#include <vector>    // for std::vector

#include "P528.h"

// Define the input keys
const std::string ModelInputKeys::d__km = "d__km";
const std::string ModelInputKeys::h_1__meter = "h_1__meter";
const std::string ModelInputKeys::h_2__meter = "h_2__meter";
const std::string ModelInputKeys::f__mhz = "f__mhz";
const std::string ModelInputKeys::T_pol = "T_pol";
const std::string ModelInputKeys::p = "p";

/*******************************************************************************
 * Top-level control function for Main Model
 * 
 * @param[in]  model_params         input parameter struct
 * @param[out] result               Result object
 * @return                          Return code
 ******************************************************************************/
int CallModel(
    ModelParams &model_params, Result &result
) {
    int rtn;

    rtn = P528(
        model_params.d__km,
        model_params.h_1__meter,
        model_params.h_2__meter,
        model_params.f__mhz,
        model_params.T_pol,
        model_params.p,
        &result
    );

    return rtn;
}

/*******************************************************************************
 * Parse input stream (file or string stream) to model parameter struct.
 * 
 * @param[in]  stream               Input stream containing Model parameters
 * @param[out] params               Model input parameter struct
 * @return                          Return code
 ******************************************************************************/
DrvrReturnCode ParseModelInputStream(
    std::istream &stream, ModelParams &params
) {
    CommaSeparatedIterator it(stream);
    DrvrReturnCode rtn = DRVR__SUCCESS;
    std::string key, value, errMsg;
    while (it) {
        std::tie(key, value) = *it;
        if (key.compare(ModelInputKeys::d__km) == 0) {
            rtn = ParseDouble(value, params.d__km);
            if (rtn == DRVRERR__PARSE)
                rtn = DRVRERR__PARSE_DIST;
        } else if (key.compare(ModelInputKeys::h_1__meter) == 0) {
            rtn = ParseDouble(value, params.h_1__meter);
            if (rtn == DRVRERR__PARSE)
                rtn = DRVRERR__PARSE_H1;
        } else if (key.compare(ModelInputKeys::h_2__meter) == 0) {
            rtn = ParseDouble(value, params.h_2__meter);
            if (rtn == DRVRERR__PARSE)
                rtn = DRVRERR__PARSE_H2;
        } else if (key.compare(ModelInputKeys::f__mhz) == 0) {
            rtn = ParseDouble(value, params.f__mhz);
            if (rtn == DRVRERR__PARSE)
                rtn = DRVRERR__PARSE_FREQ;
        } else if (key.compare(ModelInputKeys::T_pol) == 0) {
            int polarization_int;
            rtn = ParseInteger(value, polarization_int);
            if (rtn == DRVRERR__PARSE) {
                rtn = DRVRERR__PARSE_POL;
            } else {
                if (polarization_int >= 0 && polarization_int < 2 ){
                    params.T_pol = static_cast<Polarization>(polarization_int);
                } else {
                    std::cerr << key << " is an invalid polarization value."
                              << std::endl;
                    rtn = DRVRERR__PARSE_POL;
                }
            }
        } else if (key.compare(ModelInputKeys::p) == 0) {
            rtn = ParseDouble(value, params.p);
            if (rtn == DRVRERR__PARSE)
                rtn = DRVRERR__PARSE_PTime;
        } else {
            std::cerr << "Unknown parameter: " << key << std::endl;
            rtn = DRVRERR__PARSE;
        }

        if (rtn != DRVR__SUCCESS) {
            std::cerr << GetDrvrReturnStatusMsg(rtn) << std::endl;
            return rtn;
        }
        ++it;
    }
    return rtn;
}

/*******************************************************************************
 * Parse Model input parameter file
 * 
 * @param[in]  in_file              Path to input parameter file
 * @param[out] params               Model parameter struct
 * @return                          Return code
 ******************************************************************************/
DrvrReturnCode ParseInputFile(
    const std::string &in_file, ModelParams &params
) {
    std::ifstream file(in_file);
    if (!file) {
        std::cerr << "Failed to open file " << in_file << std::endl;
        return DRVRERR__OPENING_INPUT_FILE;
    }
    return ParseModelInputStream(file, params);
}

/*******************************************************************************
 * Write Model inputs to the report file
 * 
 * @param[in] fp      Output stream, a text file open for writing
 * @param[in] params  Model parameter struct
 ******************************************************************************/
void WriteInputs(
    std::ofstream &fp, const ModelParams &params
) {
    fp PRINT ModelInputKeys::d__km SETW13 params.d__km << "[km]";
    fp PRINT ModelInputKeys::h_1__meter SETW13 params.h_1__meter << "[m]";
    fp PRINT ModelInputKeys::h_2__meter SETW13 params.h_2__meter << "[m]";
    fp PRINT ModelInputKeys::f__mhz SETW13 params.f__mhz << "[MHz]";
    fp PRINT ModelInputKeys::T_pol SETW13 static_cast<int>(
        params.T_pol
    );
    fp PRINT ModelInputKeys::p SETW13 params.p << "[hPa]";
}

/*******************************************************************************
 * Write Model outputs to the report file
 * 
 * @param[in]  fp           Output stream, a text file open for writing
 * @param[in]  result       Model output result struct
 ******************************************************************************/
void WriteModelOutputs(
    std::ofstream &fp, const Result &result
) {
    fp PRINT "Propagation mode" SETW13 result.propagation_mode;
    fp PRINT "Warnings" SETW13 result.warnings;
    fp PRINT "Path distance used in calculations" SETW13 std::fixed
        << std::setprecision(5) << result.d__km << "[km]";
    fp PRINT "Total loss" SETW13 std::fixed
        << std::setprecision(5) << result.A__db << "[dB]";
    fp PRINT "Free space path loss" SETW13 std::fixed
        << std::setprecision(5) << result.A_fs__db << "[dB]";
    fp PRINT "Atmospheric absorption loss" SETW13 std::fixed
        << std::setprecision(5) << result.A_a__db << "[dB]";
    fp PRINT "Elevation angle at the low terminal" SETW13 std::fixed
        << std::setprecision(5) << result.theta_h1__rad << "[rad]";
}



