#pragma once
#include "RE/Skyrim.h"
#include "SidePolicy.h"

namespace MountSide
{
	[[nodiscard]] bool         RegisterConditions();
	void                       Reset();
	void                       Capture(RE::Actor* a_horse, RE::TESObjectREFR* a_player, Action a_action, Side a_side);
	[[nodiscard]] bool         IsRight(RE::TESObjectREFR* a_ref, Action a_action);
	[[nodiscard]] RE::BSString Describe(RE::TESObjectREFR* a_ref, Action a_action);
}
