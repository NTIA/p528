/** @file ReturnCodes.cpp
 * Maps status messages to library return codes.
 */

#include "P528.h"

#ifdef _WIN32
    // Ensure strcpy_s is available on Windows
    #ifndef __STDC_LIB_EXT1__
        #define __STDC_LIB_EXT1__
    #endif
    #ifndef __STDC_WANT_LIB_EXT1__
        #define __STDC_WANT_LIB_EXT1__ 1
    #endif
#endif

#include <cstring>        // for strcpy_s
#include <string>         // for std::string
#include <unordered_map>  // for std::unordered_map

namespace ITS {
namespace ITU {
namespace PSeries {
namespace P528 {

/*******************************************************************************
 * Get library name
 * 
  * @return         Library name.
  ******************************************************************************/
std::string GetLibraryName() {
    return LIBRARY_NAME;
}

/*******************************************************************************
 * Get library version
 * 
  * @return         Library version.
  ******************************************************************************/
std::string GetLibraryVersion() {
    return LIBRARY_VERSION;
}

/*******************************************************************************
 * Get an error message string from a return code.
 * 
 * @param[in] code  Integer return code.
 * @return          A status message corresponding to the input code.
 ******************************************************************************/
std::string GetReturnStatus(int code) {
    static const std::unordered_map<ReturnCodes, std::string> messages = {
        {SUCCESS, "Successful execution"},
        {ERROR_VALIDATION__D_KM, "Path distance must be >= 0 km"},
        {ERROR_VALIDATION__H_1, "Low terminal height must be >= 1.5 meters"},
        {ERROR_VALIDATION__H_2, "High terminal height must be >= 1.5 meters"},
        {ERROR_VALIDATION__TERM_GEO, "Low terminal must be <= high terminal height"},
        {ERROR_VALIDATION__F_MHZ_LOW, "Frequency must be >= 100 MHz"},
        {ERROR_VALIDATION__F_MHZ_HIGH, "Frequency must be <= 30,000 MHz"},
        {ERROR_VALIDATION__PERCENT_LOW, "Time percentage must be >= 1"},
        {ERROR_VALIDATION__PERCENT_HIGH, "Time percentage must be <= 99"},
        {ERROR_VALIDATION__POLARIZATION, "T_pol must equal POLARIZATION__HORIZONTAL or POLARIZATION__VERTICAL"},
        {ERROR_HEIGHT_AND_DISTANCE, "Terminals are occupying the same point in space (they are the same height and 0 km apart)"},
        {SUCCESS_WITH_WARNINGS, "Successful execution, but there were warnings triggered."},
    };
    // Construct status message
    std::string msg = LIBRARY_NAME;
    msg += " v";
    msg += LIBRARY_VERSION;
    if (code == SUCCESS) {
        msg += " Status: ";
    } else {
        msg += " Error: ";
    }

    auto it = messages.find(static_cast<ReturnCodes>(code));
    if (it != messages.end()) {
        msg += it->second;
    } else {
        msg += "Undefined return code";
    }
    return msg;
}

/*******************************************************************************
 * Get an error message string (as C-style string) from a return code.
 * 
 * @param[in] code  Integer return code.
 * @return          A status message corresponding to the input code.
 ******************************************************************************/
char *GetReturnStatusCharArray(const int code) {
    const std::string msg = GetReturnStatus(code);
    char *c_msg = new char[msg.size() + 1];
#ifdef _WIN32
    strcpy_s(c_msg, msg.size() + 1, msg.c_str());
#else
    strcpy(c_msg, msg.c_str());
#endif
    return c_msg;
}

/*******************************************************************************
 * Free the memory allocated by GetReturnStatusCharArray
 * 
 * @param[in] c_msg  The status message C-style string to delete
 ******************************************************************************/
void FreeCharArray(char *c_msg) {
    delete[] c_msg;
}

/*******************************************************************************
 * Get library name string (as C-style string).
 * 
 * @return          Library name string.
 ******************************************************************************/
char *GetLibraryNameCharArray() {
    const std::string msg = LIBRARY_NAME;
    char *c_msg = new char[msg.size() + 1];
#ifdef _WIN32
    strcpy_s(c_msg, msg.size() + 1, msg.c_str());
#else
    strcpy(c_msg, msg.c_str());
#endif
    return c_msg;
}

/*******************************************************************************
 * Get library version string (as C-style string).
 * 
 * @return          Library version string.
 ******************************************************************************/
char *GetLibraryVersionCharArray() {
    const std::string msg = LIBRARY_VERSION;
    char *c_msg = new char[msg.size() + 1];
#ifdef _WIN32
    strcpy_s(c_msg, msg.size() + 1, msg.c_str());
#else
    strcpy(c_msg, msg.c_str());
#endif
    return c_msg;
}

}  // namespace P528
}  // namespace PSeries
}  // namespace ITU
}  // namespace ITS