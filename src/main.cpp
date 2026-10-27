#include "Hooks.h"
#include "MountSide.h"
#include "Runtime.h"

namespace
{
	void MessageHandler(SKSE::MessagingInterface::Message* a_message)
	{
		if (!a_message) {
			return;
		}
		switch (a_message->type) {
		case SKSE::MessagingInterface::kPostLoad:
			if (MountSide::RegisterConditions()) {
				Hooks::Install();
			}
			break;
		case SKSE::MessagingInterface::kPreLoadGame:
		case SKSE::MessagingInterface::kNewGame:
			MountSide::Reset();
			break;
		default:
			break;
		}
	}
}

// CommonLib supplies both pre/post-629 layouts and Address Library formats 1/2/5.
SKSEPluginInfo(
		.Version = Plugin::VERSION,
	.Name = Plugin::NAME,
	.Author = "Wuerfel",
	.StructCompatibility = SKSE::StructCompatibility::Independent,
	.RuntimeCompatibility = SKSE::VersionIndependence::AddressLibrary);

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse)
{
	if (!a_skse || a_skse->IsEditor() || !Runtime::IsSupported(a_skse->RuntimeVersion())) {
		return false;
	}
	SKSE::Init(a_skse, true);
	spdlog::set_pattern("[%H:%M:%S:%e] [%l] %v");
	spdlog::set_level(spdlog::level::info);
	spdlog::flush_on(spdlog::level::info);
	logger::info("{} {} FLATRIM on runtime {}", Plugin::NAME, Plugin::VERSION.string(), a_skse->RuntimeVersion().string());
	const auto messaging = SKSE::GetMessagingInterface();
	if (!messaging || !messaging->RegisterListener(MessageHandler)) {
		logger::critical("Unable to register SKSE message listener");
		return false;
	}
	return true;
}
