#pragma once
#include "ObjectReference.h"

namespace Skyrim
{
	class Hazard : public ObjectReference
	{
	public:
		constexpr static FormType FORM_TYPE = FormType::REF_HAZARD;

	public:
		virtual void Hazard_a2();
		virtual void Hazard_a3();
		virtual void Hazard_a4();

		float getAge() const { return get_member<float>(this, offsetof_age); }
		float getDuration() const { return get_member<float>(this, offsetof_duration); }

		//Hazard radius uses UI units, not game units (inconsistent with Explosion and Projectile). 
		//The conversion factor appears to be 22.
		float getRadius() const { return 22.0f * get_member<float>(this, offsetof_radius); }

	private:
		static var_offset offsetof_age;
		static var_offset offsetof_duration;
		static var_offset offsetof_radius;
	};
}
