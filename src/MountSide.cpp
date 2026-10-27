#include "MountSide.h"

namespace MountSide
{
	namespace
	{
		using Clock = std::chrono::steady_clock;

		struct Snapshot
		{
			RE::ObjectRefHandle horse;
			RE::ObjectRefHandle player;
			Action              action{ Action::Mount };
			Side                side{ Side::Left };
			Clock::time_point   time{};
		};

		Snapshot   snapshot;
		std::mutex stateMutex;

		[[nodiscard]] bool IsValid(RE::TESObjectREFR* a_ref, Action a_action)
		{
			const auto horse = snapshot.horse.get();
			const auto player = snapshot.player.get();
			return a_ref && horse && player && Matches(a_ref->GetFormID(), player->GetFormID(), horse->GetFormID(), a_action, snapshot.action, std::chrono::duration<double>(Clock::now() - snapshot.time).count());
		}
	}

	void Capture(RE::Actor* a_horse, RE::TESObjectREFR* a_player, Action a_action, Side a_side)
	{
		if (!a_horse || !a_player) {
			return;
		}
		std::scoped_lock locker(stateMutex);
		snapshot = { a_horse->CreateRefHandle(), a_player->CreateRefHandle(), a_action, a_side, Clock::now() };
	}

	void Reset()
	{
		std::scoped_lock locker(stateMutex);
		snapshot = {};
	}

	bool IsRight(RE::TESObjectREFR* a_ref, Action a_action)
	{
		std::scoped_lock locker(stateMutex);
		if (!IsValid(a_ref, a_action)) {
			return false;
		}
		return snapshot.side == Side::Right;
	}

	RE::BSString Describe(RE::TESObjectREFR* a_ref, Action a_action)
	{
		std::scoped_lock locker(stateMutex);
		if (!IsValid(a_ref, a_action)) {
			return "No matching recent activation";
		}
		return snapshot.side == Side::Right ? "Right (latched at activation)" : "Left (latched at activation)";
	}
}
