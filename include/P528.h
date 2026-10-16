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
 * max(x,y). Returns y if x=y
 */
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
/**
 * @brief
 * min(x,y). Returns y if x=y
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
    CONST_MODE__SEARCH        = 0, ///< Code is currnetly running TranshorizonSearch.
    CONST_MODE__DIFFRACTION   = 1, ///< Code returned TranshorizonSearch with CASE_1.
    CONST_MODE__SCATTERING    = 2, ///< Code returned TranshorizonSearch with CASE_2.
/** Smooth Earth diffraction losses are predicted to be less than the losses
 * due to troposcatter, and the diffraction model is guaranteed to intersect the
 * troposcatter model at some distance ≥ 𝑑′. The transhorizon region propagation
 * loss is physically consistent.*/
    CASE_1    = 1,
    CASE_2    = 2, /**< he diffraction line is parallel to the tangent of the troposcatter model.
However, the transhorizon region propagation loss could be not physically consistent,
i.e., there is a potential discontinuity*/

    PROP_MODE__NOT_SET= 0, ///< Program did not finish running.
    PROP_MODE__LOS    = 1, ///< Path is in the Line-of-Sight range.
    PROP_MODE__DIFFRACTION    = 2, ///< Diffraction propagation dominates
    PROP_MODE__SCATTERING     = 3, ///< Scattering propagation dominates
};


/** List of valid polarizations */
enum Polarization {
    POLARIZATION__HORIZONTAL  = 0, ///< antenna has horizontal polarization
    POLARIZATION__VERTICAL    = 1, ///< antenna has vertical polarization
};


/**
 * @brief
 * y value (second index) for NakagamiRiceCurves
 */
#define Y_pi_99_INDEX  16


/**
 * @brief return codes
 */
enum ReturnCodes{
    SUCCESS   =  0, ///< Successful execution
    ERROR_VALIDATION__D_KM    =  1, ///< Path distance must be >= 0 km
    ERROR_VALIDATION__H_1     =  2, ///< Low terminal height must be >= 1.5 meters
    ERROR_VALIDATION__H_2     =  3, ///< High terminal height must be >= 1.5 meters
    ERROR_VALIDATION__TERM_GEO=  4, ///< Low terminal must be <= high terminal height
    ERROR_VALIDATION__F_MHZ_LOW       =  5, ///< Frequency must be >= 100 MHz
    ERROR_VALIDATION__F_MHZ_HIGH      =  6, ///< Frequency must be <= 30,000 MHz
    ERROR_VALIDATION__PERCENT_LOW     =  7, ///< Time percentage must be >= 1
    ERROR_VALIDATION__PERCENT_HIGH    =  8, ///< Time percentage must be <= 99
    ERROR_VALIDATION__POLARIZATION    =  9, ///< T_pol must equal POLARIZATION__HORIZONTAL or POLARIZATION__VERTICAL
    ERROR_HEIGHT_AND_DISTANCE =  10,///< Terminals are occupying the same point in space (they are the same height and 0 km apart)
    SUCCESS_WITH_WARNINGS     =  11,///< Successful execution, but there were warnings triggered.
};

/**
 * @brief warnings
 */
enum Warnings {
    WARNING__NO_WARNINGS      =  0x00, ///< No warning flags
    WARNING__DFRAC_TROPO_REGION       =  0x01, ///< Warning that the diffraction and troposcatter model may not be physically consistent with each other. Caution should be taken when using the result
    WARNING__HEIGHT_LIMIT_H_1 =  0x02, ///< Terminal 1 height above limit defined within in-force Recommendation text. Results should not be intepreted as an official prediction of Recommendation ITU-R P.528, but instead treated as informative
    WARNING__HEIGHT_LIMIT_H_2 =  0x04, ///< Terminal 2 height above limit defined within in-force Recommendation text. Results should not be intepreted as an official prediction of Recommendation ITU-R P.528, but instead treated as informative
};

//P
// CLASSES
////////////////////////////////////////////////////////////////////////////////

/** Class to process Nakagami-Rice distributions table in \ref data.cpp . */
class data
{ 
public:
    const static vector<double> P;    /**< Time Percentage. */
    const static vector<vector<double>> NakagamiRiceCurves; /**< Values. */
    const static vector<int> K;      /**< Row parameter */
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
    double d_ML__km;       /**< Maximum line of sight distance */
    double d_0__km;        /**< Internal param.  See Recommendation text */
    double d_d__km;        /**< Distance where smooth earth diffraction is 0 dB */
    ///@}
};

/** Terminal variables */
struct Terminal
{
    /** @name Height
     */
    ///@{
    double h_r__km;         /**< Real terminal height */
    double h_e__km;         /**< Effective terminal height */
    double delta_h__km;     /**< Internal terminal param.  See Recommendation text */
    ///@}

    /** @name Distances
     */
    ///@{
    double d_r__km;         /**< Ray traced horizon distance */
    double a__km;           /**< Total ray path length to horizon */
    ///@}

    /** @name Angles
     */
    ///@{
    double phi__rad;        /**< Central angle between the terminal and its smooth earth horizon */
    double theta__rad;      /**< Incident angle of the grazing ray at the terminal */
    ///@}

    /** @name Losses
     */
    ///@{
    double A_a__db;         /**< Median atmospheric absorption loss, in dB */
    ///@}
};

/** Structure to hold Line of outputs. */
struct LineOfSightParams
{
    /** @name Heights
     */
    ///@{
    double z__km[2];       /**< A terminal geometric parameter */
    ///@}

    /** @name Distances
     */
    ///@{
    double d__km;          /**< Path distance between terminals */
    double r_0__km;        /**< Direct ray length */
    double r_12__km;       /**< Indirect ray length */
    double D__km[2];       /**< A terminal geometric parameter */
    ///@}

    /** @name Angles
     */
    ///@{
    double theta_h1__rad;  /**< Take-off angle from low terminal to high terminal, in rad */
    double theta_h2__rad;  /**< Take-off angle from high terminal to low terminal, in rad */
    double theta[2];       /**< A terminal geometric parameter */
    ///@}

    /** @name Misc
     */
    ///@{
    double a_a__km;        /**< Adjusted earth radius */
    double delta_r__km;    /**< Ray length path difference */
    double A_LOS__db;      /**< Loss due to LOS path */
    ///@}
};

/** Troposcatter parameters. */
struct TroposcatterParams
{
    /** @name Distances
     */
    ///@{
    double d_s__km;        /**< Scattering distance */
    double d_z__km;        /**< Half the scattering distance */

    double h_v__km;        /**< Height of the common volume cross-over point */
    ///@}

    /** @name Angles
     */
    ///@{
    double theta_s;        /**< Scattering angle */
    double theta_A;        /**< cross-over angle */
    ///@}

    /** @name Losses
     */
    ///@{
    double A_s__db;        /**< Troposcattter Loss */
    double A_s_prev__db;   /**< Troposcatter Loss of Previous Test Point */
    ///@}

    /** @name Misc
     */
    ///@{
    double M_s;            /**< Troposcatter Line Slope */
    ///@}
};

/** Structure to hold  outputs. */
struct Result {
    int propagation_mode;  /**< Mode of propagation */
    int warnings;          /**< Warning messages */
    double d__km;          /**< Path distance used in calculations */
    double A__db;          /**< Total loss */
    double A_fs__db;       /**< Free space path loss */
    double A_a__db;        /**< Atmospheric absorption loss, in dB */
    double theta_h1__rad;  /**< Elevation angle of the ray at the low terminal, in rad */
};

//
// FUNCTIONS
///////////////////////////////////////////////

// Private Functions
void GetPathLoss(double psi, Path *path, double f__mhz, double psi_limit, 
    double A_dML__db, double A_d_0__db, int T_pol, LineOfSightParams* params, double *R_Tg);
void RayOptics(Terminal *terminal_1, Terminal *terminal_2, double psi, LineOfSightParams *result);
void TerminalGeometry(double f__mhz, Terminal *terminal);
void Troposcatter(Path *path, Terminal *terminal_1, Terminal *terminal_2, 
    double d__km, double f__mhz, TroposcatterParams *tropo_params);
void TranshorizonSearch(Path* path, Terminal *terminal_1, Terminal *terminal_2, 
    double f__mhz, double A_dML__db, double *M_d, double *A_d0, 
    double* d_crx__km, int* MODE, int* warnings);
double LinearInterpolation(double x1, double y1, double x2, double y2, double x);
void ReflectionCoefficients(double psi, double f__mhz, int T_pol, double* R_g, double* phi_g);
void LineOfSight(Path* path, Terminal* terminal_1, Terminal* terminal_2, LineOfSightParams* los_params, double f__mhz, double A_dML__db,
    double p, double d__km, int T_pol, Result *result, double *K_LOS);
double SmoothEarthDiffraction(double d_1__km, double d_2__km, double f__mhz, double d_0__km, int T_pol);
double InverseComplementaryCumulativeDistributionFunction(double q);
void LongTermVariability(double d_r1__km, double d_r2__km, double d__km, double f__mhz, double time_percentage, 
    double f_theta_h, double PL, double *Y_e__db, double *A_Y);
double CombineDistributions(double A_M, double A_i, double B_M, double B_i, double p);
int ValidateInputs(double d__km, double h_1__meter, double h_2__meter, double f__mhz, 
    int T_pol, double p, int* warnings);
std::string GetReturnStatus(const int code);
std::string GetLibraryName();
std::string GetLibraryVersion();

// Public Functions
PROPLIB_API int P528(double d__km, double h_1__meter, double h_2__meter, double f__mhz,
    int T_pol, double p, Result *result);
PROPLIB_API int P528_Ex(double d__km, double h_1__meter, double h_2__meter, double f__mhz,
    int T_pol, double p, Result* result, Terminal* terminal_1, Terminal* terminal_2,
    TroposcatterParams* tropo, Path* path, LineOfSightParams* los_params);
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
