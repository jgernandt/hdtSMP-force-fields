#pragma once
#include "ObjRef_traits.h"
#include "Explosion.h"

namespace jg
{
	template<>
	struct obj_traits<Skyrim::Explosion> : obj_traits<Skyrim::ObjectReference>
	{
		using base_type = Skyrim::ExplosionBase;

		constexpr static bool hasAge() { return true; }
		static float age(const Skyrim::Explosion& obj) { return obj.getAge(); }

		//static float duration(const Explosion& obj) { return obj.getDuration(); }//explosion duration is not used?

		static float force(const Skyrim::Explosion& obj)
		{
			auto base = obj.getBaseFormAs<base_type>();
			return base ? base->getForce() : 0.0f;
		}

		static float length(const Skyrim::Explosion& obj) { return radius(obj); }
		static float radius(const Skyrim::Explosion& obj) { return obj.getRadius(); }
	};
}
