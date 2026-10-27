#pragma once
#include <cmath>
#include <cstdint>

namespace MountSide
{
	enum class Side
	{
		Left,
		Right
	};
	enum class Action
	{
		Mount,
		Dismount
	};

	// Skyrim yaw is clockwise from +Y; local +X is the horse's right.
	[[nodiscard]] inline Side Approach(float a_dx, float a_dy, float a_yaw) noexcept
	{
		if (!std::isfinite(a_dx) || !std::isfinite(a_dy) || !std::isfinite(a_yaw)) {
			return Side::Left;
		}
		const auto right = a_dx * std::cos(a_yaw) - a_dy * std::sin(a_yaw);
		// The paired start marks are symmetric (+/-63.479 on local X).
		// The sign selects the nearer mark, including approaches from front/back.
		// Treat sub-millimeter rounding around the centerline as a left-side tie.
		return right > 1.0e-3f ? Side::Right : Side::Left;
	}

	[[nodiscard]] inline Side CameraSide(float a_forwardX, float a_forwardY, float a_horseYaw) noexcept
	{
		if (!std::isfinite(a_forwardX) || !std::isfinite(a_forwardY) || !std::isfinite(a_horseYaw)) {
			return Side::Left;
		}
		const auto length = std::hypot(a_forwardX, a_forwardY);
		if (length < 1.0e-4f) {
			return Side::Left;
		}
		const auto right = (a_forwardX * std::cos(a_horseYaw) - a_forwardY * std::sin(a_horseYaw)) / length;
		// Five degrees around straight ahead/behind avoid jitter choosing right.
		constexpr float deadZone = 0.087155743f;
		return right > deadZone ? Side::Right : Side::Left;
	}

	[[nodiscard]] constexpr bool Matches(std::uint32_t a_ref, std::uint32_t a_player, std::uint32_t a_horse,
		Action a_requested, Action a_captured, double a_age) noexcept
	{
		return a_horse != 0 && (a_ref == a_player || a_ref == a_horse) && a_requested == a_captured && a_age >= 0.0 && a_age <= 8.0;
	}
}
