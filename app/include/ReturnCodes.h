/** @file ReturnCodes.h
 * Defines return codes for the driver
 */
#pragma once

#include <string>  // for std::string

/*******************************************************************************
 * Return Codes defined by this driver software (128-255)
 ******************************************************************************/
// clang-format off
enum DrvrReturnCode {
    /** @name Primary Return Codes
     */
    ///@{
    /**! Successful execution */
    DRVR__SUCCESS = 128,
    /**! Indicates driver should exit successfully */
    DRVR__RETURN_SUCCESS,
    /**! No value provided for given argument */
    DRVRERR__MISSING_OPTION,
    /**! Unknown option specified */
    DRVRERR__INVALID_OPTION,
    /**! Failed to open the input file for reading */
    DRVRERR__OPENING_INPUT_FILE, 
    /**! Failed to open the output file for writing */
    DRVRERR__OPENING_OUTPUT_FILE,       
    ///@}
    /** @name Input File Parsing Errors
     */
    ///@{
    /**! Failed parsing inputs; unknown parameter */
    DRVRERR__PARSE = 160,               
    /**! Failed to parse frequency value */
    DRVRERR__PARSE_FREQ,
    /**! Failed to parse time percentage value */
    DRVRERR__PARSE_PTime,
    /**! Failed to parse path distance value */
    DRVRERR__PARSE_DIST,
    /**! Failed to parse polarization value */
    DRVRERR__PARSE_POL,
    /**! Failed to parse height of the low terminal value */
    DRVRERR__PARSE_H1,
    /**! Failed to parse height of the high terminal value */
    DRVRERR__PARSE_H2,
    ///@}
    /** @name Validation Errors
     */
    ///@{ 
    /**! Input file not specified */
    DRVRERR__VALIDATION_IN_FILE = 192,
    /**! Output file not specified */
    DRVRERR__VALIDATION_OUT_FILE,
    /**! Model not specified */
    DRVRERR__VALIDATION_MODEL,
    ///@}
};
// clang-format on

std::string GetDrvrReturnStatusMsg(int code);
