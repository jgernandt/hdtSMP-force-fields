#pragma once
#include <cstdint>

namespace Skyrim
{
	using FormID = std::uint32_t;

	enum class FormType : std::uint32_t
	{
		NONE = 0,
		PROJECTILE = 50,
		HAZARD = 51,
		REF_OBJECT = 61,
		REF_MISSILE = 63,
		REF_ARROW = 64,
		REF_GRENADE = 65,
		REF_BEAM = 66,
		REF_FLAME = 67,
		REF_CONE = 68,
		REF_BARRIER = 69,
		REF_HAZARD = 70,
		EXPLOSION = 87,
	};
}
