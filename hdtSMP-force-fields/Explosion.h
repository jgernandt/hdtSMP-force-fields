#pragma once
#include "ObjectReference.h"

namespace Skyrim
{
	class ExplosionBase : public Form
	{
	public:
		constexpr static FormType FORM_TYPE = FormType::EXPLOSION;

	public:
		float getForce() const { return get_member<float>(this, offsetof_force); }
		float getRadius() const { return get_member<float>(this, offsetof_radius); }

	private:
		constexpr static std::size_t offsetof_ExplosionBase = 0x98;
		constexpr static std::size_t offsetof_force = offsetof_ExplosionBase + 0x30;
		constexpr static std::size_t offsetof_radius = offsetof_ExplosionBase + 0x38;
	};

	class Explosion : public ObjectReference
	{
	public:
		float getAge() const { return get_member<float>(this, offsetof_age); }
		float getDuration() const { return get_member<float>(this, offsetof_duration); }
		float getRadius() const { return get_member<float>(this, offsetof_radius); }

	private:
		static var_offset offsetof_age;
		static var_offset offsetof_duration;
		static var_offset offsetof_radius;
	};
}
