#pragma once
#include "ObjRef_traits.h"
#include "Projectile.h"

namespace jg
{
	template<>
	struct obj_traits<Skyrim::Projectile> : obj_traits<Skyrim::ObjectReference>
	{
		constexpr static Skyrim::FormType BASE_TYPE = Skyrim::FormType::PROJECTILE;
		using base_type = Skyrim::ProjectileBase;

		constexpr static bool hasAge() { return true; }
		static float age(const Skyrim::Projectile& obj) { return obj.getAge(); }

		static float force(const Skyrim::Projectile& obj)
		{
			auto base = obj.getBaseFormAs<base_type>();
			return base ? base->getForce() : 0.0f;
		}

		static float length(const Skyrim::Projectile& obj) { return radius(obj); }
		static float radius(const Skyrim::Projectile& obj)
		{
			auto base = obj.getBaseFormAs<base_type>();
			return base ? base->getRadius() : 0.0f;
		}

		static float scale(const Skyrim::Projectile& obj, float s)
		{
			return obj_traits<Skyrim::ObjectReference>::scale(obj, s) * obj.getScale();
		}
	};

	template<>
	struct obj_traits<Skyrim::ConeProjectile> : obj_traits<Skyrim::Projectile>
	{
		static float scale(const Skyrim::ConeProjectile& obj, float s)
		{
			return obj_traits<Skyrim::Projectile>::scale(obj, s) * obj.getSpread();
		}
	};
}
