#pragma once

#include <omp.h>
#include <json.hpp> // JSON

// Include the libraries

// ----- LibRapid ----------------
#include <librapid>

// ----- eigen -------------------
#include <Eigen>

// ----- xtensor -----------------
#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>

// Namespace aliases
namespace lrc  = librapid;
namespace json = nlohmann;
using namespace nlohmann::literals;

// Utility functions
std::string formatTime(double time);
bool jsonContains(const json::json &j, const std::string &key);
std::string tableToString(const json::json &table);
