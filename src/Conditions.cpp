#include "API/OpenAnimationReplacerAPI-Conditions.h"
#include "MountSide.h"

namespace MountSide
{
	namespace
	{
		template <Action action>
		class SideCondition final : public Conditions::CustomCondition
		{
		public:
			static constexpr std::string_view CONDITION_NAME = action == Action::Mount ? "BetterHorseMounting_MountRight" : "BetterHorseMounting_DismountRight";

			[[nodiscard]] RE::BSString GetName() const override { return CONDITION_NAME.data(); }
			[[nodiscard]] RE::BSString GetDescription() const override
			{
				return action == Action::Mount ?
				           "Player approached the captured horse from its right. Latched before alignment; expires after 8 seconds." :
				           "Camera looked to the horse's right when dismount was requested. Near forward/backward or vertical defaults to left.";
			}
			[[nodiscard]] REL::Version GetRequiredVersion() const override { return REL::Version{ 1, 0, 0, 0 }; }
			[[nodiscard]] RE::BSString GetCurrent(RE::TESObjectREFR* a_ref) const override { return Describe(a_ref, action); }

		private:
			bool EvaluateImpl(RE::TESObjectREFR* a_ref, RE::hkbClipGenerator*, void*) const override
			{
				return IsRight(a_ref, action);
			}
		};

		template <Action action>
		bool RegisterCondition()
		{
			using Condition = SideCondition<action>;
			const auto result = OAR_API::Conditions::AddCustomCondition<Condition>();
			if (result != OAR_API::Conditions::APIResult::OK) {
				logger::error("Failed to register {} (result={})", Condition::CONDITION_NAME, static_cast<int>(result));
				return false;
			}
			return true;
		}
	}

	bool RegisterConditions()
	{
		if (!OAR_API::Conditions::GetAPI()) {
			logger::error("Open Animation Replacer Conditions API unavailable; hooks remain inactive");
			return false;
		}
		const auto mount = RegisterCondition<Action::Mount>();
		const auto dismount = RegisterCondition<Action::Dismount>();
		return mount && dismount;
	}
}
