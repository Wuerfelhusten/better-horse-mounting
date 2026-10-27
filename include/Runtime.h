#pragma once

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

namespace Runtime
{
	[[nodiscard]] constexpr bool IsSupported(REL::Version a_version) noexcept
	{
		return a_version.major() == 1 && a_version >= SKSE::RUNTIME_SSE_1_5_97 && a_version <= SKSE::RUNTIME_SSE_1_7_104;
	}

	[[nodiscard]] inline std::size_t ActivateButtonIndex() noexcept
	{
		// Same version-dependent slot as CommonLib's PlayerInputHandler::ProcessButton.
		return REL::VersionShift(0x04, RE::PlayerInputHandler::kAE1799AddedVFuncCount, SKSE::RUNTIME_SSE_1_7_99);
	}
}
