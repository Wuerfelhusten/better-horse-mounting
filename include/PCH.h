#pragma once
#include "Plugin.h"
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include <chrono>
#include <mutex>
using namespace std::literals;
namespace logger = SKSE::log;

#if !defined(ENABLE_SKYRIM_SE) || !defined(ENABLE_SKYRIM_AE) || defined(ENABLE_SKYRIM_VR)
#	error Better Horse Mounting requires a FLATRIM CommonLib build (SE + AE, without VR).
#endif
