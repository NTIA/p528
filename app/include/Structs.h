/** @file Structs.h
 * Contains data structures and type macros used by this software
*/
#pragma once



#include <string>  // for std::string


/////////////////////////////
// Data Structures

/** Parameters provided to the command line driver */
struct DrvrParams {
    std::string in_file = "";              ///< Input file
    std::string out_file = "";             ///< Output file
};


/** Input parameters for the Model */
struct ModelParams {
    double d__km;       ///< %Path distance, in km
    double h_1__meter;  ///< Height of the low terminal, in meters
    double h_2__meter;  ///< Height of the high terminal, in meters
    double f__mhz;      ///< Frequency, in MHz
    int T_pol;          ///< Polarization type
    double p;           ///< Time percentage
};

/** Key names for Model input file parameters */
struct ModelInputKeys {
    static const std::string d__km;       ///< %Path distance, in km
    static const std::string h_1__meter;  ///< Height of the low terminal, in meters
    static const std::string h_2__meter;  ///< Height of the high terminal, in meters
    static const std::string f__mhz;      ///< Frequency, in MHz
    static const std::string T_pol;       ///< Polarization type
    static const std::string p;           ///< Time percentage
};

