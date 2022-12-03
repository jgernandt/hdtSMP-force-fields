#pragma once
#include "ObjectReference.h"

namespace Skyrim
{
	class ProjectileBase : public Form
	{
	public:
		constexpr static FormType FORM_TYPE = FormType::PROJECTILE;

	public:
		float getDuration() const { return get_member<float>(this, offsetof_duration); }
		float getForce() const { return get_member<float>(this, offsetof_force); }
		float getRadius() const { return get_member<float>(this, offsetof_radius); }
		float getSpeed() const { return get_member<float>(this, offsetof_speed); }

	private:
		constexpr static std::size_t offsetof_ProjectileBase = 0x80;
		constexpr static std::size_t offsetof_speed = offsetof_ProjectileBase + 8;
		constexpr static std::size_t offsetof_force = offsetof_ProjectileBase + 0x48;
		constexpr static std::size_t offsetof_radius = offsetof_ProjectileBase + 0x6c;
		constexpr static std::size_t offsetof_duration = offsetof_ProjectileBase + 0x70;
	};

	class Projectile : public ObjectReference
	{
	public:
		virtual bool isMissile() { return false; }
		virtual bool isGrenade() { return false; }
		virtual bool isFlame() { return false; }
		virtual bool isBeam() { return false; }
		virtual void Projectile_a6();//supposed to be isCone, but they misnamed the override?
		virtual bool isBarrier() { return false; }
		virtual void Projectile_a8();
		virtual void Projectile_a9();
		virtual void Projectile_aa();
		virtual void Projectile_ab() = 0;
		virtual void Projectile_ac();
		virtual void Projectile_ad();
		virtual void Projectile_ae();
		virtual void Projectile_af();
		virtual void Projectile_b0();
		virtual void Projectile_b1();
		virtual void Projectile_b2();
		virtual void Projectile_b3();
		virtual void Projectile_b4();
		virtual void Projectile_b5();
		virtual void Projectile_b6();
		virtual void Projectile_b7();
		virtual void Projectile_b8();
		virtual void Projectile_b9();
		virtual void Projectile_ba();
		virtual void Projectile_bb();
		virtual void Projectile_bc();
		virtual void Projectile_bd();
		virtual void Projectile_be();
		virtual void Projectile_bf();
		virtual void Projectile_c0();
		virtual void Projectile_c1();

		float getAge() const { return get_member<float>(this, offsetof_age); }
		float getScale() const { return get_member<float>(this, offsetof_scale); }
		float getSpeedMult() const { return get_member<float>(this, offsetof_speedMult); }

	private:
		static var_offset offsetof_speedMult;
		static var_offset offsetof_age;
		static var_offset offsetof_scale;

	protected:
		static var_offset sizeof_Projectile;
	};


	class ConeProjectile : public Projectile
	{
	public:
		constexpr static FormType FORM_TYPE = FormType::REF_CONE;

	public:
		virtual void ConeProjectile_c2();//supposed to be Projectile_a6 (isCone), but they misnamed it?

		float getSpread() const
		{
			if (auto base = getBaseFormAs<ProjectileBase>()) {
				return 1.0f + getTanSpread() * getAge() * base->getSpeed() * getSpeedMult() / getInitialRadius();
			}
			else {
				return 1.0f;
			}
		}
		float getInitialRadius() const { return get_member<float>(this, offsetof_initialRadius); }
		float getTanSpread() const { return get_member<float>(this, offsetof_tanSpread); }

	private:
		static var_offset offsetof_tanSpread;
		static var_offset offsetof_initialRadius;
	};
}
