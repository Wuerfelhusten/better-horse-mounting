#include "Hooks.h"
#include "MountSide.h"
#include "Runtime.h"

namespace Hooks
{
	namespace
	{
		struct HorseActivate
		{
			static bool thunk(RE::TESNPC* a_this, RE::TESObjectREFR* a_target, RE::TESObjectREFR* a_activator,
				std::uint8_t a_arg3, RE::TESBoundObject* a_object, std::int32_t a_count)
			{
				const auto player = a_activator && a_activator->IsPlayerRef() ? a_activator->As<RE::Actor>() : nullptr;
				const auto horse = a_target ? a_target->As<RE::Actor>() : nullptr;
				const bool capture = player && horse && horse->IsHorse() && !player->IsOnMount();
				if (capture) {
					const auto delta = player->GetPosition() - horse->GetPosition();
					MountSide::Capture(horse, a_activator, MountSide::Action::Mount, MountSide::Approach(delta.x, delta.y, horse->GetAngleZ()));
				}
				const auto result = func(a_this, a_target, a_activator, a_arg3, a_object, a_count);
				if (capture && !result) {
					MountSide::Reset();
				}
				return result;
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		struct ActivateButton
		{
			static void thunk(RE::ActivateHandler* a_this, RE::ButtonEvent* a_event, RE::PlayerControlsData* a_data)
			{
				const auto               player = RE::PlayerCharacter::GetSingleton();
				RE::NiPointer<RE::Actor> horse;
				if (a_event && a_event->IsDown() && player && player->IsOnMount() && player->GetMount(horse) && horse && horse->IsHorse()) {
					auto       side = MountSide::Side::Left;
					const auto camera = RE::PlayerCamera::GetSingleton();
					if (camera && camera->cameraRoot) {
						const auto direction = camera->cameraRoot->world.rotate.GetVectorY();
						side = MountSide::CameraSide(direction.x, direction.y, horse->GetAngleZ());
					}
					MountSide::Capture(horse.get(), player, MountSide::Action::Dismount, side);
				}
				func(a_this, a_event, a_data);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};
	}

	void Install()
	{
		REL::Relocation<std::uintptr_t> npcTable{ RE::VTABLE_TESNPC[0] };
		HorseActivate::func = npcTable.write_vfunc(0x37, HorseActivate::thunk);
		REL::Relocation<std::uintptr_t> activateTable{ RE::VTABLE_ActivateHandler[0] };
		const auto                      buttonIndex = Runtime::ActivateButtonIndex();
		ActivateButton::func = activateTable.write_vfunc(buttonIndex, ActivateButton::thunk);
	}
}
