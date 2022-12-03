#pragma once
#include "ObjRef_traits.h"
#include "Hazard.h"

namespace jg
{
	template<>
	struct obj_traits<Skyrim::Hazard> : obj_traits<Skyrim::ObjectReference>
	{
		constexpr static bool hasAge() { return true; }
		static float age(const Skyrim::Hazard& obj) { return obj.getAge(); }

		static float duration(const Skyrim::Hazard& obj) { return obj.getDuration(); }

		static float length(const Skyrim::Hazard& obj) { return radius(obj); }
		static float radius(const Skyrim::Hazard& obj) { return obj.getRadius(); }
	};
}
