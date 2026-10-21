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
    /**! %Path distance, in km */
    double d__km;
    /**! Height of the low terminal, in meters */
    double h_1__meter;
    /**! Height of the high terminal, in meters */
    double h_2__meter;
    /**! Frequency, in MHz */
    double f__mhz;
    /**! Polarization type */
    int T_pol;
    /**! Time percentage */
    double p;
};

/** Key names for Model input file parameters */
struct ModelInputKeys {
    /**! %Path distance, in km */
    static const std::string d__km;       
    /**! Height of the low terminal, in meters */
    static const std::string h_1__meter;  
    /**! Height of the high terminal, in meters */
    static const std::string h_2__meter;  
    /**! Frequency, in MHz */
    static const std::string f__mhz;      
    /**! Polarization type */
    static const std::string T_pol;       
    /**! Time percentage */
    static const std::string p;
};

