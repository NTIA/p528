
typedef int(__stdcall *p528func)(double d__km, double h_1__meter, double h_2__meter, 
    double f__mhz, int T_pol, double p, struct Result* result);

//
// CONSTANTS
///////////////////////////////////////////////

#define     MODE_POINT                              0
#define     MODE_CURVE                              1
#define     MODE_TABLE                              2
#define     MODE_VERSION                            3
#define     TIME_SIZE                               26
#define     CURVE_POINTS                            1801

//
// GENERAL ERRORS AND RETURN VALUES
///////////////////////////////////////////////

#define     NOT_SET                                 -1
#define     SUCCESS                                 0
#define     SUCCESS_WITH_WARNINGS                   11
#define     DRVR__RETURN_SUCCESS                    1000

#define     DRVRERR__UNKNOWN                        1001
#define     DRVRERR__DLL_LOADING                    1002
#define     DRVRERR__MAJOR_VERSION_MISMATCH         1003
#define     DRVRERR__INVALID_OPTION                 1004
#define     DRVRERR__GETP528_FUNC_LOADING           1005
// Parsing Errors (1000-1099)
#define     DRVRERR__PARSE_H1_HEIGHT                1010
#define     DRVRERR__PARSE_H2_HEIGHT                1011
#define     DRVRERR__PARSE_F_FREQUENCY              1012
#define     DRVRERR__PARSE_D_DISTANCE               1013
#define     DRVRERR__PARSE_P_PERCENTAGE             1014
#define     DRVRERR__PARSE_MODE_VALUE               1015
#define     DRVRERR__PARSE_TPOL_POLARIZATION        1016
// Validation Errors (1100-1199)
#define     DRVRERR__VALIDATION_MODE                1100
#define     DRVRERR__VALIDATION_F                   1101
#define     DRVRERR__VALIDATION_P                   1102
#define     DRVRERR__VALIDATION_D                   1103
#define     DRVRERR__VALIDATION_H1                  1104
#define     DRVRERR__VALIDATION_H2                  1105
#define     DRVRERR__VALIDATION_OUT_FILE            1106
#define     DRVRERR__VALIDATION_TPOL                1107

//
// WARNINGS
///////////////////////////////////////////////

#define WARNING__NO_WARNINGS                0x00
#define WARNING__DFRAC_TROPO_REGION         0x01
#define WARNING__HEIGHT_LIMIT_H_1           0x02
#define WARNING__HEIGHT_LIMIT_H_2           0x04

//
// DATA STRUCTURES
///////////////////////////////////////////////

struct Result {
    int propagation_mode;       // Mode of propagation
    int warnings;               // Warning messages

    double d__km;               // Path distance used in calculations
    double A__db;               // Total loss
    double A_fs__db;            // Free space path loss
    double A_a__db;             // Atmospheric absorption loss, in dB

    double theta_h1__rad;       // Elevation angle of the ray at the low terminal, in rad
};

struct DrvrParams {
    double h_1__meter = NOT_SET;  // Low terminal height (meter), 1.5 <= h_1__meter <= 20 000 AND h_1__meter <= h2__meter
    double h_2__meter = NOT_SET;  // High terminal height (meter), 1.5 <= h_2__meter <= 20 000 AND h_1__meter <= h2__meter
    double f__mhz = NOT_SET;      // Frequency (MHz), 100 <= f___mhz <= 30 000
    double p = NOT_SET;           // Time percentage, 1 <= p <= 99
    double d__km = NOT_SET;       // Path distance (km), 0 <= d__km
    int T_pol = NOT_SET;          // Polarization

    int mode = NOT_SET;           // Mode (POINT, CURVE, TABLE)

    char out_file[256] = { 0 };   // Output file
};

//
// FUNCTIONS
///////////////////////////////////////////////

int ParseArguments(int argc, char** argv, DrvrParams* params);
void Lowercase(char* argv);
bool Match(const char* opt, char* arg);
void Help();
int ParseErrorMsgHelper(const char* opt, int err);
void GetDLLVersionInfo();
void GetDrvrVersionInfo();
int ValidateInputs(DrvrParams* params);
int Validate_RequiredErrMsgHelper(const char* opt, int err);
int LoadDLL();
int CallP528_POINT(DrvrParams* params);
int CallP528_CURVE(DrvrParams* params);
int CallP528_TABLE(DrvrParams* params);
