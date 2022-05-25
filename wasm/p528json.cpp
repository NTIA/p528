#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

#include <memory>

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "../include/p528.h"
#include <nlohmann/json.hpp>

static const char *jsonToString(const nlohmann::json &json) {
  std::string s =
      json.dump(-1, ' ', false, nlohmann::detail::error_handler_t::ignore);
  auto str = (char *)malloc(s.size() + 1);
  strcpy(str, s.c_str());
  return str;
}

EMSCRIPTEN_KEEPALIVE
extern "C" const char *calcP528(const char *requestJsonString) {
  nlohmann::json response;

  try {
    nlohmann::json request = nlohmann::json::parse(requestJsonString);

    /// <summary>
    /// Height of the low terminal, in user defined units
    /// </summary>
    double h_1__user_units = request["h_1__user_units"];

    /// <summary>
    /// Height of the high terminal, in user define units
    /// </summary>
    double h_2__user_units = request["h_2__user_units"];

    /// <summary>
    /// Time percentage
    /// </summary>
    double time = request["time"];

    /// <summary>
    /// Polarization
    /// </summary>
    int pol = request["Polarization"];

    /// <summary>
    /// Frequency, in MHz
    /// </summary>
    double f__mhz = request["f__mhz"];

    std::vector<double> d__user_units_vec;
    std::vector<double> L_btl__db_vec;
    std::vector<double> L_fs__db_vec;
    std::vector<int> PropModes_vec;

    bool _fullResolution = false;
    double min = 0.0;
    double max = 1800.0;

    /// <summary>
    /// Default number of steps in plot
    /// </summary>
    int _steps = 50;

    // execute jobs
    // iterate on user-specified units (km or n miles)
    double d_step__user_units = _fullResolution ? 1 : (max - min) / _steps;
    double d__user_units = min;

    int warn = SUCCESS;

    while (d__user_units <= max) {

      Result result;
      auto rtn = P528(d__user_units, h_1__user_units, h_2__user_units, f__mhz,
                      pol, time, &result);

      // Ignore 'ERROR_HEIGHT_AND_DISTANCE' for visualization.  Just relates
      // to the d__km = 0 point and will return 0 dB result
      if (rtn != ERROR_HEIGHT_AND_DISTANCE && rtn != SUCCESS &&
          rtn != SUCCESS_WITH_WARNINGS)

        // record the result
        d__user_units_vec.push_back(result.d__km);
      L_btl__db_vec.push_back(result.A__db);
      L_fs__db_vec.push_back(result.A_fs__db);
      PropModes_vec.push_back(result.propagation_mode);

      // add warning code
      warn |= result.warnings;

      // iterate
      d__user_units += d_step__user_units;
    }

    nlohmann::json response;

    nlohmann::json j_d__user_units(d__user_units_vec);
    nlohmann::json j_L_btl__db(L_btl__db_vec);
    nlohmann::json j_L_fs__db(L_fs__db_vec);
    nlohmann::json j_PropModes(PropModes_vec);

    /// <summary>
    /// Mode of propagation
    /// </summary>
    response["ModeOfPropagation"] = j_PropModes;

    /// <summary>
    /// Warning flags
    /// </summary>
    response["warnings"] = warn;

    /// <summary>
    /// Path distance used in calculations, in km
    /// </summary>
    response["d__km"] = j_d__user_units;

    /// <summary>
    /// Basic transmission loss, in dB
    /// </summary>
    response["A__db"] = L_btl__db_vec;

    /// <summary>
    /// Free space basic transmission loss, in dB
    /// </summary>
    response["A_fs__db"] = j_L_fs__db;

    /// <summary>
    /// Median atmospheric absorption loss, in dB
    /// </summary>
    response["A_a__db"] = 0;

    /// <summary>
    /// Elevation angle of the ray at the low terminal, in rad
    /// </summary>
    response["theta_h1__rad"] = 0;

    return jsonToString(response);
  } catch (std::exception &ex) {
    std::cout << "std::exception" << std::endl;
    std::cout << ex.what() << std::endl;
    response["error"] = "std::exception: " + std::string(ex.what());
  }
  return jsonToString(response);
}