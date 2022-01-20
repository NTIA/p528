#include <ctime>
#include <iostream>
#include <string.h>
#include <Windows.h>
#include "P528Drvr.h"

/*=============================================================================
 |
 |  Description:  This driver allows the user to execute the P.528 model
 |                DLL and generate results.  For full details and examples
 |                on how to use it, please see the readme.txt file.
 |
 *===========================================================================*/

 // Local globals
HINSTANCE hLib;
p528func dllP528;

int dllVerMajor = NOT_SET;
int dllVerMinor = NOT_SET;
int drvrVerMajor = NOT_SET;
int drvrVerMinor = NOT_SET;

wchar_t buf[TIME_SIZE];

/*=============================================================================
 |
 |  Description:  Main function of the P.528 driver executable
 |
 *===========================================================================*/
int main(int argc, char** argv) {
    int rtn;
    DrvrParams params;

    // Get the time
    time_t t = time(NULL);
    _wctime_s(buf, TIME_SIZE, &t);

    rtn = ParseArguments(argc, argv, &params);
    if (rtn == DRVR__RETURN_SUCCESS)
        return SUCCESS;
    if (rtn) {
        Help();
        return rtn;
    }

    if (params.mode != MODE_VERSION) {
        rtn = ValidateInputs(&params);
        if (rtn)
        {
            printf_s("\n");
            Help();
            return rtn;
        }
    }

    rtn = LoadDLL();
    if (rtn)
        return rtn;

    switch (params.mode) {
    case MODE_POINT:
        rtn = CallP528_POINT(&params);
        break;
    case MODE_CURVE:
        rtn = CallP528_CURVE(&params);
        break;
    case MODE_TABLE:
        rtn = CallP528_TABLE(&params);
        break;
    case MODE_VERSION:
        printf_s("*******************************************************\n");
        printf_s("Institute for Telecommunications Sciences - Boulder, CO\n");
        printf_s("\tP.528 Driver Version: %i.%i\n", drvrVerMajor, drvrVerMinor);
        printf_s("\tP.528 DLL Version: %i.%i\n", dllVerMajor, dllVerMinor);
        wprintf_s(L"Time: %s", buf);
        printf_s("*******************************************************\n");
        break;
    default:
        Help();
    }

    FreeModule(hLib);

    return rtn;
}

/*=============================================================================
 |
 |  Description:  Generates a data table of P.528 values, per the format of
 |                the data files currently distributed from the Study
 |                Group 3 website.
 |
 |        Input:  params        - Structure with user input parameters
 |
 |      Returns:  P.528 DLL return code
 |
 *===========================================================================*/
int CallP528_TABLE(DrvrParams* params) {
    Result result;

    FILE* fp;
    int err = fopen_s(&fp, params->out_file, "w");
    if (err != 0) {
        printf_s("Error opening output file.  Exiting.\n");
        return err;
    }
    else {
        fprintf_s(fp, "%fMHz / Lb(%f) dB\n", params->f__mhz, params->p);
        fprintf_s(fp, ",h2(m),1000,1000,1000,1000,1000,10000,10000,10000,10000,10000,10000,20000,20000,20000,20000,20000,20000,20000\n");
        fprintf_s(fp, ",h1(m),1.5,15,30,60,1000,1.5,15,30,60,1000,10000,1.5,15,30,60,1000,10000,20000\n");
        fprintf_s(fp, "D (km),FSL\n");

        for (int d__km = 0; d__km < CURVE_POINTS; d__km++) {
            fprintf_s(fp, "%i", d__km);

            dllP528(d__km, 1.5, 1000, params->f__mhz, params->T_pol, params->p, &result);    // FSL
            fprintf_s(fp, ",%.3f", result.A_fs__db);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 15, 1000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 30, 1000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 60, 1000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 1000, 1000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 1.5, 10000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 15, 10000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 30, 10000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 60, 10000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 1000, 10000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 10000, 10000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 1.5, 20000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 15, 20000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 30, 20000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 60, 20000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 1000, 20000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 10000, 20000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f", result.A__db);

            dllP528(d__km, 20000, 20000, params->f__mhz, params->T_pol, params->p, &result);
            fprintf_s(fp, ",%.3f\n", result.A__db);
        }
    }

    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  Generates data points for a P.528 loss-vs-distance curve
 |
 |        Input:  params        - Structure with user input parameters
 |
 |      Returns:  P.528 DLL return code
 |
 *===========================================================================*/
int CallP528_CURVE(DrvrParams* params) {
    Result result;
    int rtn;

    double A__dbs[CURVE_POINTS];
    double A_fs__dbs[CURVE_POINTS];
    int warnings[CURVE_POINTS];

    // Gather data points
    for (int d__km = 0; d__km < CURVE_POINTS; d__km++) {
        rtn = dllP528(d__km, params->h_1__meter, params->h_2__meter, params->f__mhz, params->T_pol, params->p, &result);

        A__dbs[d__km] = result.A__db;
        A_fs__dbs[d__km] = result.A_fs__db;
        warnings[d__km] = result.warnings;

        if (rtn != SUCCESS && rtn != SUCCESS_WITH_WARNINGS)
            break;
    }

    // Print results to file
    FILE* fp;
    int err = fopen_s(&fp, params->out_file, "w");
    if (err != 0) {
        printf("Error opening output file.  Exiting.\n");
        return err;
    }
    else {
        fprintf_s(fp, "p528_x86.dll Version,%i.%i\n", dllVerMajor, dllVerMinor);
        fprintf_s(fp, "P528Drvr_x86.exe Version,%i.%i\n", drvrVerMajor, drvrVerMinor);
        fwprintf_s(fp, L"Date Generated,%s", buf);
        fprintf_s(fp, "\n");
        fprintf_s(fp, "Inputs\n");
        fprintf_s(fp, "h_1__meter,%f\n", params->h_1__meter);
        fprintf_s(fp, "h_2__meter,%f\n", params->h_2__meter);
        fprintf_s(fp, "f__mhz,%f\n", params->f__mhz);
        fprintf_s(fp, "p,%f\n", params->p);
        fprintf_s(fp, "T_pol,%i\n", params->T_pol);
        fprintf_s(fp, "\n");

        if (rtn != SUCCESS && rtn != SUCCESS_WITH_WARNINGS) {
            fprintf_s(fp, "P.528 returned error,%i\n", rtn);
        }
        else {
            fprintf_s(fp, "Results\n");

            fprintf_s(fp, "Distance (km)");
            for (int d__km = 0; d__km < CURVE_POINTS; d__km++)
                fprintf_s(fp, ",%i", d__km);
            fprintf_s(fp, "\n");

            fprintf_s(fp, "Free Space Loss (dB),%.3f", A_fs__dbs[0]);
            for (int i = 1; i < CURVE_POINTS; i++)
                fprintf_s(fp, ",%.3f", A_fs__dbs[i]);
            fprintf_s(fp, "\n");

            fprintf_s(fp, "Basic Transmission Loss (dB),%.3f", A__dbs[0]);
            for (int i = 1; i < CURVE_POINTS; i++)
                fprintf_s(fp, ",%.3f", A__dbs[i]);
            fprintf_s(fp, "\n");

            fprintf_s(fp, "Warnings,0x%x", warnings[0]);
            for (int i = 1; i < CURVE_POINTS; i++)
                fprintf_s(fp, ",0x%x", warnings[i]);
            fprintf_s(fp, "\n");
        }

        fclose(fp);
    }

    return rtn;
}

/*=============================================================================
 |
 |  Description:  Executes P.528 at a single point
 |
 |        Input:  params        - Structure with user input parameters
 |
 |      Returns:  P.528 DLL return code
 |
 *===========================================================================*/
int CallP528_POINT(DrvrParams* params) {
    Result result;

    int rtn = dllP528(params->d__km, params->h_1__meter, params->h_2__meter, params->f__mhz, params->T_pol, params->p, &result);

    if (params->mode == MODE_POINT && strlen(params->out_file) == 0)
    {
        // Don't write to a file - print to screen instead
        if (rtn != SUCCESS && rtn != SUCCESS_WITH_WARNINGS) {
            printf_s("P.528 Returned Error Code: %i\n", rtn);
        }
        else {
            printf_s("Free Space Loss (dB): %.3f\n", result.A_fs__db);
            printf_s("Basic Transmission Loss (dB): %.3f\n", result.A__db);
            printf_s("Warning Flags: 0x%x\n", result.warnings);
        }
    }
    else {
        // Print results to file
        FILE* fp;
        int err = fopen_s(&fp, params->out_file, "w");
        if (err != 0) {
            printf_s("Error opening output file.  Exiting.\n");
            return err;
        }
        else {
            fprintf_s(fp, "p528_x86.dll Version,%i.%i\n", dllVerMajor, dllVerMinor);
            fprintf_s(fp, "P528Drvr_x86.exe Version,%i.%i\n", drvrVerMajor, drvrVerMinor);
            fwprintf_s(fp, L"Date Generated,%s", buf);
            fprintf_s(fp, "\n");
            fprintf_s(fp, "Inputs\n");
            fprintf_s(fp, "h_1__meter,%f\n", params->h_1__meter);
            fprintf_s(fp, "h_2__meter,%f\n", params->h_2__meter);
            fprintf_s(fp, "f__mhz,%f\n", params->f__mhz);
            fprintf_s(fp, "d__km,%f\n", params->d__km);
            fprintf_s(fp, "p,%f\n", params->p);
            fprintf_s(fp, "T_pol,%i\n", params->T_pol);
            fprintf_s(fp, "\n");

            if (rtn != SUCCESS && rtn != SUCCESS_WITH_WARNINGS) {
                fprintf_s(fp, "P.528 returned error,%i\n", rtn);
            }
            else {
                fprintf_s(fp, "Results\n");
                fprintf_s(fp, "Free Space Loss (dB),%.3f\n", result.A_fs__db);
                fprintf_s(fp, "Basic Transmission Loss (dB),%.3f\n", result.A__db);
                fprintf_s(fp, "DLL Return Code,%i\n", rtn);
                fprintf_s(fp, "Warning Flags,0x%x\n", result.warnings);
            }

            fclose(fp);
        }
    }

    return rtn;
}

/*=============================================================================
 |
 |  Description:  Loads the P.528 DLL
 |
 |        Input:  [void]
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
int LoadDLL() {
    hLib = LoadLibrary(TEXT("p528_x86.dll"));

    if (hLib == NULL)
        return DRVRERR__DLL_LOADING;

    GetDLLVersionInfo();
    GetDrvrVersionInfo();
    if (dllVerMajor != drvrVerMajor)
        return DRVRERR__MAJOR_VERSION_MISMATCH;

    // Grab the functions in the DLL
    dllP528 = (p528func)GetProcAddress((HMODULE)hLib, "P528");
    if (dllP528 == nullptr)
        return DRVRERR__GETP528_FUNC_LOADING;

    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  Get the version information of the P.528 DLL
 |
 |        Input:  [void]
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void GetDLLVersionInfo() {
    DWORD  verHandle = NULL;
    UINT   size = 0;
    LPBYTE lpBuffer = NULL;
    DWORD  verSize = GetFileVersionInfoSize(TEXT("p528_x86.dll"), &verHandle);

    if (verSize != NULL)
    {
        LPSTR verData = new char[verSize];

        if (GetFileVersionInfo(TEXT("p528_x86.dll"), verHandle, verSize, verData))
        {
            if (VerQueryValue(verData, TEXT("\\"), (VOID FAR * FAR*) & lpBuffer, &size))
            {
                VS_FIXEDFILEINFO* verInfo = (VS_FIXEDFILEINFO*)lpBuffer;
                if (verInfo->dwSignature == 0xfeef04bd)
                {
                    dllVerMajor = (verInfo->dwFileVersionMS >> 16) & 0xffff;
                    dllVerMinor = (verInfo->dwFileVersionMS >> 0) & 0xffff;
                }
            }
        }

        delete[] verData;
    }

    return;
}

/*=============================================================================
 |
 |  Description:  Get the version information of this driver
 |
 |        Input:  [void]
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void GetDrvrVersionInfo()
{
    DWORD  verHandle = NULL;
    UINT   size = 0;
    LPBYTE lpBuffer = NULL;
    WCHAR fileName[MAX_PATH];

    GetModuleFileName(NULL, fileName, MAX_PATH);
    DWORD  verSize = GetFileVersionInfoSize(fileName, &verHandle);

    if (verSize != NULL)
    {
        LPSTR verData = new char[verSize];

        if (GetFileVersionInfo(fileName, verHandle, verSize, verData))
        {
            if (VerQueryValue(verData, L"\\", (VOID FAR * FAR*) & lpBuffer, &size))
            {
                if (size)
                {
                    VS_FIXEDFILEINFO* verInfo = (VS_FIXEDFILEINFO*)lpBuffer;
                    if (verInfo->dwSignature == 0xfeef04bd)
                    {
                        drvrVerMajor = (verInfo->dwFileVersionMS >> 16) & 0xffff;
                        drvrVerMinor = (verInfo->dwFileVersionMS >> 0) & 0xffff;
                    }
                }
            }
        }

        delete[] verData;
    }

    return;
}

/*=============================================================================
 |
 |  Description:  Helper function to format and print error messages
 |                encountered during command argument parsing
 |
 |        Input:  opt       - Command flag in error
 |                err       - Error code
 |
 |      Returns:  Error code
 |
 *===========================================================================*/
int ParseErrorMsgHelper(const char* opt, int err) {
    printf_s("DrvrErr %i: Unable to parse %s value.\n", err, opt);
    return err;
}

/*=============================================================================
 |
 |  Description:  Parse the command line arguments
 |
 |        Input:  argc      - Number of arguments
 |                argv      - Command line arguments
 |
 |       Output:
 |                params    - Structure with user input parameters
 |
 |      Returns:  SUCCESS, or error code encountered
 |
 *===========================================================================*/
int ParseArguments(int argc, char** argv, DrvrParams* params)
{
    for (int i = 1; i < argc; i++) {
        Lowercase(argv[i]);

        if (Match("-h1", argv[i])) {
            if (sscanf_s(argv[i + 1], "%lf", &(params->h_1__meter)) != 1)
                return ParseErrorMsgHelper("-h1 [height]", DRVRERR__PARSE_H1_HEIGHT);
            i++;
        }
        else if (Match("-h2", argv[i])) {
            if (sscanf_s(argv[i + 1], "%lf", &(params->h_2__meter)) != 1)
                return ParseErrorMsgHelper("-h2 [height]", DRVRERR__PARSE_H2_HEIGHT);
            i++;
        }
        else if (Match("-f", argv[i])) {
            if (sscanf_s(argv[i + 1], "%lf", &(params->f__mhz)) != 1)
                return ParseErrorMsgHelper("-f [frequency]", DRVRERR__PARSE_F_FREQUENCY);
            i++;
        }
        else if (Match("-d", argv[i])) {
            if (sscanf_s(argv[i + 1], "%lf", &(params->d__km)) != 1)
                return ParseErrorMsgHelper("-d [distance]", DRVRERR__PARSE_D_DISTANCE);
            i++;
        }
        else if (Match("-p", argv[i])) {
            if (sscanf_s(argv[i + 1], "%lf", &(params->p)) != 1)
                return ParseErrorMsgHelper("-p [percentage]", DRVRERR__PARSE_P_PERCENTAGE);
            i++;
        }
        else if (Match("-tpol", argv[i])) {
            if (sscanf_s(argv[i + 1], "%i", &(params->T_pol)) != 1)
                return ParseErrorMsgHelper("-tpol [polarization]", DRVRERR__PARSE_TPOL_POLARIZATION);
            i++;
        }
        else if (Match("-o", argv[i])) {
            sprintf_s(params->out_file, "%s", argv[i + 1]);
            i++;
        }
        else if (Match("-mode", argv[i]))
        {
            Lowercase(argv[i + 1]);

            if (Match("point", argv[i + 1]))
                params->mode = MODE_POINT;
            else if (Match("curve", argv[i + 1]))
                params->mode = MODE_CURVE;
            else if (Match("table", argv[i + 1]))
                params->mode = MODE_TABLE;
            else
                return ParseErrorMsgHelper("-mode [mode]", DRVRERR__PARSE_MODE_VALUE);

            i++;
        }
        else if (Match("-v", argv[i])) {
            params->mode = MODE_VERSION;
            return SUCCESS;
        }
        else if (Match("-h", argv[i])) {
            Help();
            return DRVR__RETURN_SUCCESS;
        }
        else {
            printf_s("Unknown option: %s\n", argv[i]);
            return DRVRERR__INVALID_OPTION;
        }
    }

    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  Validate that the required inputs are present for the
 |                mode specified by the user.  This function DOES NOT
 |                check the validity of the parameter values - only that
 |                required parameters have been specified by the user
 |
 |        Input:  params        - Structure with user input parameters
 |
 |      Returns:  SUCCESS, or error code encountered
 |
 *===========================================================================*/
int ValidateInputs(DrvrParams* params) {
    if (params->f__mhz == NOT_SET)
        return Validate_RequiredErrMsgHelper("-f", DRVRERR__VALIDATION_F);

    if (params->p == NOT_SET)
        return Validate_RequiredErrMsgHelper("-p", DRVRERR__VALIDATION_P);

    if (params->T_pol == NOT_SET)
        return Validate_RequiredErrMsgHelper("-tpol", DRVRERR__VALIDATION_P);

    if (params->mode == NOT_SET)
        return Validate_RequiredErrMsgHelper("-mode", DRVRERR__VALIDATION_MODE);

    if (params->mode == MODE_POINT) {
        if (params->d__km == NOT_SET)
            return Validate_RequiredErrMsgHelper("-d", DRVRERR__VALIDATION_D);
    }

    if (params->mode == MODE_POINT || params->mode == MODE_CURVE) {
        if (params->h_1__meter == NOT_SET)
            return Validate_RequiredErrMsgHelper("-h1", DRVRERR__VALIDATION_H1);

        if (params->h_2__meter == NOT_SET)
            return Validate_RequiredErrMsgHelper("-h2", DRVRERR__VALIDATION_H2);
    }

    if (params->mode == MODE_CURVE || params->mode == MODE_TABLE) {
        if (strlen(params->out_file) == 0)
            return  Validate_RequiredErrMsgHelper("-o", DRVRERR__VALIDATION_OUT_FILE);
    }

    return SUCCESS;
}

/*=============================================================================
 |
 |  Description:  Helper function to format and print error messages
 |                encountered during validation of input parameters
 |
 |        Input:  opt       - Command flag in error
 |                err       - Error code
 |
 |      Returns:  Error code
 |
 *===========================================================================*/
int Validate_RequiredErrMsgHelper(const char* opt, int err) {
    printf_s("DrvrError %i: Option %s is required but was not provided\n", err, opt);
    return err;
}

/*=============================================================================
 |
 |  Description:  Convert the char array to lower case
 |
 | Input/Output:  argv      - value
 |
 |      Returns:  [Void]
 |
 *===========================================================================*/
void Lowercase(char* argv) {
    for (int i = 0; i < strlen(argv); i++)
        argv[i] = tolower(argv[i]);
}

/*=============================================================================
 |
 |  Description:  Compare to strings to see if they match
 |
 |        Input:  opt       - Given char array
 |                arg       - Expected char array
 |
 |      Returns:  True/False
 |
 *===========================================================================*/
bool Match(const char* opt, char* arg) {
    if (strcmp(opt, arg) == 0)
        return true;
    return false;
}

/*=============================================================================
 |
 |  Description:  Print help instructions to the terminal
 |
 |        Input:  [Void]
 |
 |      Returns:  [Void]
 |
 *===========================================================================*/
void Help() {
    printf_s("\n");
    printf_s("Usage: P528Drvr_x86.exe [Options]\n");
    printf_s("Options (not case sensitive)\n");
    printf_s("\t-h    :: Displays help\n");
    printf_s("\t-v    :: Displays version information\n");
    printf_s("\t-h1   :: Height of low terminal, in meters\n");
    printf_s("\t-h2   :: Height of high terminal, in meters\n");
    printf_s("\t-f    :: Frequency, in MHz\n");
    printf_s("\t-p    :: Percentage\n");
    printf_s("\t-tpol :: Polarization\n");
    printf_s("\t-d    :: Path distance, in km\n");
    printf_s("\t-o    :: Output file name\n");
    printf_s("\t-mode :: Mode of operation [POINT, CURVE, TABLE]\n");
    printf_s("\n");
    printf_s("Examples:\n");
    printf_s("\tP528Drvr_x86.exe -mode POINT -h1 10 -h2 20000 -f 3000 -p 50 -tpol 1 -d 600\n");
    printf_s("\tP528Drvr_x86.exe -mode CURVE -h1 15 -h2 15000 -f 450 -p 10 -tpol 0 -o curve.csv\n");
    printf_s("\tP528Drvr_x86.exe -mode TABLE -f 6500 -p 90 -tpol 1 -o table.csv\n");
    printf_s("\n");
};