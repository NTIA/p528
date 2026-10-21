/** @file P528.h
 * Interface header for this library
 */
#pragma once

#include <string>  // for std::string
#include <vector>  // for std::vector
#include <algorithm>

using namespace std;


#ifndef DOXYGEN_SHOULD_SKIP
    #ifndef PROPLIB_API
        #ifdef _WIN32
            #define PROPLIB_API extern "C" __declspec(dllexport)
        #else
            #define PROPLIB_API extern "C"
        #endif
    #endif
#endif


namespace ITS {
namespace ITU {
namespace PSeries {
namespace P528 {

/**
 * @brief
 * max(x,y).
 * @param x The first value to compare.
 * @param y The second value to compare.
 * @return The maximum of the two values.
 */
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

/**
 * @brief
 * min(x,y).
 * @param x The first value to compare.
 * @param y The second value to compare.
 * @return The minimum of the two values.
 */
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/**
 * @brief
 * Approximate value of @f$ \pi @f$.
 */
constexpr double PI = 3.1415926535897932384;

/**
 * @brief
 * The average Earth radius, in km.
 * @note
 * References:
 *     - Recommendation ITU‑R P.528‑5 "A propagation prediction method for
 * aeronautical mobile and radionavigation services using the VHF, UHF and SHF
 * bands", Annex 2, Section 2.
 */
constexpr double a_0__km = 6371.0;

/**
 * @brief
 * The effective Earth radius, in km
 * @note
 * References:
 *     - Recommendation ITU‑R P.528‑5 "A propagation prediction method for
 * aeronautical mobile and radionavigation services using the VHF, UHF and SHF
 * bands", Annex 2, Section 2.
 */
constexpr double a_e__km = 9257.0;

/**
 * @brief
 * Surface index of refractivity
 *
 * @note
 * References:
 *     - Recommendation ITU‑R P.528‑4 "A propagation prediction method for
 * aeronautical mobile and radionavigation services using the VHF, UHF and SHF
 * bands", Annex 2, Section 2.
 */
constexpr double N_s = 341;

/**
 * @brief
 * The relative dieletric constant (set corresponding to average ground)
 * @note
 * References:
 *     - Recommendation ITU‑R P.528‑5 "A propagation prediction method for
 * aeronautical mobile and radionavigation services using the VHF, UHF and SHF
 * bands", Annex 2, Section 2.
 */
constexpr double epsilon_r = 15.0;

/**
 * @brief
 * Conductivity, in S/m (set corresponding to average ground)
 * @note
 * References:
 *     - Recommendation ITU‑R P.528‑5 "A propagation prediction method for
 * aeronautical mobile and radionavigation services using the VHF, UHF and SHF
 * bands", Annex 2, Section 2.
 */
constexpr double sigma = 0.005;

/**
 * @brief
 * Line of sight epsilon. Unused
 */
constexpr double LOS_EPSILON = 0.00001;

/**
 * @brief
 * computer approx for 1/3
 */
constexpr double THIRD = 1.0 / 3.0;


/**
 * @brief modes and cases used as options in algorithm.
 */
enum MODES_AND_CASES{
    /**! Code is currently running TranshorizonSearch. */
    CONST_MODE__SEARCH = 0, 
    /**! Code returned TranshorizonSearch with CASE_1. */
    CONST_MODE__DIFFRACTION = 1, 
    /**! Code returned TranshorizonSearch with CASE_2. */
    CONST_MODE__SCATTERING = 2,
    /**! Smooth Earth diffraction losses are predicted to be less than the 
     * losses due to troposcatter, and the diffraction model is guaranteed to 
     * intersect the troposcatter model at some distance &ge; d'. The
     * transhorizon region propagation loss is physically consistent.*/
    CASE_1 = 1,
    /**! The diffraction line is parallel to the tangent of the troposcatter 
     * model. However, the transhorizon region propagation loss could be not 
     * physically consistent, i.e., there is a potential discontinuity. */
    CASE_2 = 2,
    /**! Program did not finish running. */
    PROP_MODE__NOT_SET = 0, 
    /**! Path is in the Line-of-Sight range. */
    PROP_MODE__LOS = 1, 
    /**! Diffraction propagation dominates */
    PROP_MODE__DIFFRACTION  = 2, 
    /**! Scattering propagation dominates */
    PROP_MODE__SCATTERING = 3,
};


/** List of valid polarizations */
enum Polarization {
    /**! antenna has horizontal polarization */
    POLARIZATION__HORIZONTAL = 0, 
    /**! antenna has vertical polarization */
    POLARIZATION__VERTICAL = 1,
};


/**
 * @brief Index for the y value (second index) in the Nakagami-Rice curves.
 * y value (second index) for NakagamiRiceCurves
 */
#define Y_pi_99_INDEX  16


/**
 * @brief return codes
 */
enum ReturnCodes{
    SUCCESS   =  0, ///< Successful execution
    /**! Path distance must be >= 0 km */
    ERROR_VALIDATION__D_KM =  1,
    /**! Low terminal height must be >= 1.5 meters */
    ERROR_VALIDATION__H_1 =  2,
    /**! High terminal height must be >= 1.5 meters */
    ERROR_VALIDATION__H_2 =  3,
    /**! Low terminal must be <= high terminal height */
    ERROR_VALIDATION__TERM_GEO =  4,
    /**! Frequency must be >= 100 MHz */
    ERROR_VALIDATION__F_MHZ_LOW =  5,
    /**! Frequency must be <= 30,000 MHz */
    ERROR_VALIDATION__F_MHZ_HIGH =  6,
    /**! Time percentage must be >= 1 */
    ERROR_VALIDATION__PERCENT_LOW =  7,
    /**! Time percentage must be <= 99 */
    ERROR_VALIDATION__PERCENT_HIGH =  8,
    /**! T_pol must equal POLARIZATION__HORIZONTAL or POLARIZATION__VERTICAL */
    ERROR_VALIDATION__POLARIZATION =  9,
    /**! Terminals are occupying the same point in space (they are the same 
     * height and 0 km apart) */
    ERROR_HEIGHT_AND_DISTANCE =  10,
    /**! Successful execution, but there were warnings triggered. */
    SUCCESS_WITH_WARNINGS =  11,
    /**! Slant path attenuation computation failed. Likely the frequency was too
     * low. P676 gives a lower limit of 1 GHz.
     */
    ERROR__SLANT_PATH_ATTENUATION_FAILURE =  12,
};

/**
 * @brief warnings
 */
enum Warnings {
    /**! No warning flags */
    WARNING__NO_WARNINGS =  0x00,
    /**! Warning that the diffraction and troposcatter model may not be 
     * physically consistent with each other. Caution should be taken when using
     * the result */
    WARNING__DFRAC_TROPO_REGION =  0x01,
    /**! Terminal 1 height above limit defined within in-force Recommendation 
     * text. Results should not be intepreted as an official prediction of 
     * Recommendation ITU-R P.528, but instead treated as informative */
    WARNING__HEIGHT_LIMIT_H_1 =  0x02,
    /**! Terminal 2 height above limit defined within in-force Recommendation 
     * text. Results should not be intepreted as an official prediction of 
     * Recommendation ITU-R P.528, but instead treated as informative */
    WARNING__HEIGHT_LIMIT_H_2 =  0x04,
};

//P
// CLASSES
////////////////////////////////////////////////////////////////////////////////

/** Class to process Nakagami-Rice distributions table in \ref data.cpp . */
class data
{ 
public:
    /**! Time Percentage. */
    const static vector<double> P;
    /**! Values. */
    const static vector<vector<double>> NakagamiRiceCurves;
    /**! Row parameter */
    const static vector<int> K;
};

//
// DATA STRUCTURES
///////////////////////////////////////////////


/** Structure to hold Path Variables. */
struct Path
{
    /** @name Distance
     */
    ///@{
    /** Maximum line of sight distance */
    double d_ML__km;
    /** Internal param.  See Recommendation text */
    double d_0__km;
    /** Distance where smooth earth diffraction is 0 dB */
    double d_d__km;
    ///@}
};

/** Terminal variables */
struct Terminal
{ 
    /** @name Height
     */
    ///@{
    /** Real terminal height */
    double h_r__km;
    /** Effective terminal height */
    double h_e__km;
    /** Internal terminal param.  See Recommendation text */
    double delta_h__km;
    ///@}

    /** @name Distances
     */
    ///@{
    /** Ray traced horizon distance */
    double d_r__km;
    /** Total ray path length to horizon */
    double a__km;
    ///@}

    /** @name Angles
     */
    ///@{
    /** Central angle between the terminal and its smooth earth horizon */
    double phi__rad;
    /** Incident angle of the grazing ray at the terminal */
    double theta__rad;
    ///@}

    /** @name Losses
     */
    ///@{
    /** Median atmospheric absorption loss, in dB */
    double A_a__db;
    ///@}
};

/** Structure to hold Line of outputs. */
struct LineOfSightParams
{
    /** @name Heights
     */
    ///@{
    /** A terminal geometric parameter */
    double z__km[2];
    ///@}

    /** @name Distances
     */
    ///@{
    /** Path distance between terminals */
    double d__km;
    /** Direct ray length */
    double r_0__km;
    /** Indirect ray length */
    double r_12__km;
    /** A terminal geometric parameter */
    double D__km[2];
    ///@}

    /** @name Angles
     */
    ///@{
    /** Take-off angle from low terminal to high terminal, in rad */
    double theta_h1__rad;
    /** Take-off angle from high terminal to low terminal, in rad */
    double theta_h2__rad;
    /** A terminal geometric parameter */
    double theta[2];
    ///@}

    /** @name Misc
     */
    ///@{
    /** Adjusted earth radius */
    double a_a__km;
    /** Ray length path difference */
    double delta_r__km;
    /** Loss due to LOS path */
    double A_LOS__db;
    ///@}
};

/** Troposcatter parameters. */
struct TroposcatterParams
{
    /** @name Distances
     */
    ///@{
    /** Scattering distance */
    double d_s__km;
    /** Half the scattering distance */
    double d_z__km;

    /** Height of the common volume cross-over point */
    double h_v__km;
    ///@}

    /** @name Angles
     */
    ///@{
    /** Scattering angle */
    double theta_s;
    /** cross-over angle */
    double theta_A;
    ///@}

    /** @name Losses
     */
    ///@{
    /** Troposcattter Loss */
    double A_s__db;
    /** Troposcatter Loss of Previous Test Point */
    double A_s_prev__db;
    ///@}

    /** @name Misc
     */
    ///@{
    /** Troposcatter Line Slope */
    double M_s;
    ///@}
};

/** Structure to hold  outputs. */
struct Result {
    /** Mode of propagation */
    int propagation_mode;
    /** Warning messages */
    int warnings;
    /** Path distance used in calculations */
    double d__km;
    /** Total loss */
    double A__db;
    /** Free space path loss */
    double A_fs__db;
    /** Atmospheric absorption loss, in dB */
    double A_a__db;
    /** Elevation angle of the ray at the low terminal, in rad */
    double theta_h1__rad;
};

//
// FUNCTIONS
///////////////////////////////////////////////

// Private Functions
void GetPathLoss(
    double psi,
    Path *path,
    double f__mhz,
    double psi_limit,
    double A_dML__db,
    double A_d_0__db,
    int T_pol,
    LineOfSightParams* params,
    double *R_Tg);

void RayOptics(
    Terminal *terminal_1,
    Terminal *terminal_2,
    double psi,
    LineOfSightParams *result);

int TerminalGeometry(
    double f__mhz,
    Terminal *terminal);

void Troposcatter(
    Terminal *terminal_1,
    Terminal *terminal_2,
    double d__km,
    double f__mhz,
    TroposcatterParams *tropo_params);

void TranshorizonSearch(
    Path* path,
    Terminal *terminal_1,
    Terminal *terminal_2,
    double f__mhz,
    double A_dML__db,
    double *M_d,
    double *A_d0,
    double* d_crx__km,
    int* MODE,
    int* warnings);

double LinearInterpolation(
    double x1,
    double y1,
    double x2,
    double y2,
    double x);

void ReflectionCoefficients(
    double psi,
    double f__mhz,
    int T_pol,
    double* R_g,
    double* phi_g);

int LineOfSight(
    Path* path,
    Terminal* terminal_1,
    Terminal* terminal_2,
    LineOfSightParams* los_params,
    double f__mhz,
    double A_dML__db,
    double p,
    double d__km,
    int T_pol,
    Result *result,
    double *K_LOS);

double SmoothEarthDiffraction(
    double d_1__km,
    double d_2__km,
    double f__mhz,
    double d_0__km,
    int T_pol);

double InverseComplementaryCumulativeDistributionFunction(double q);

void LongTermVariability(
    double d_r1__km,
    double d_r2__km,
    double d__km,
    double f__mhz,
    double time_percentage,
    double f_theta_h,
    double PL,
    double *Y_e__db,
    double *A_Y);

double CombineDistributions(
    double A_M,
    double A_i,
    double B_M,
    double B_i,
    double p);

int ValidateInputs(
    double d__km,
    double h_1__meter,
    double h_2__meter,
    double f__mhz,
    int T_pol,
    double p,
    int* warnings);

std::string GetReturnStatus(const int code);

std::string GetLibraryName();

std::string GetLibraryVersion();


// Public Functions
PROPLIB_API int P528(
    double d__km,
    double h_1__meter,
    double h_2__meter,
    double f__mhz,
    int T_pol,
    double p,
    Result *result);

PROPLIB_API int P528_Ex(
    double d__km,
    double h_1__meter,
    double h_2__meter,
    double f__mhz,
    int T_pol,
    double p,
    Result* result,
    Terminal* terminal_1,
    Terminal* terminal_2,
    TroposcatterParams* tropo,
    Path* path,
    LineOfSightParams* los_params);

PROPLIB_API double FindKForYpiAt99Percent(double Y_pi_99__db);

PROPLIB_API double NakagamiRice(double K, double q);

PROPLIB_API char *GetReturnStatusCharArray(const int code);

PROPLIB_API void FreeCharArray(char *c_msg);

PROPLIB_API char *GetLibraryNameCharArray();

PROPLIB_API char *GetLibraryVersionCharArray();

}  // namespace P528
}  // namespace PSeries
}  // namespace ITU
}  // namespace ITS
