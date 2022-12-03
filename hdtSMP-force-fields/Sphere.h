#pragma once
#include "Concepts.h"
#include "TForceField.h"

namespace jg
{
	using SphereConcepts = TConcepts<Force, Spherical, Fluctuating<Expiring<Clock>>, Decaying, Noisy>;

	struct Sphere : SphereConcepts
	{
		void actOn(btRigidBody& body) const;
	};

	template<typename T> using SphereField = TField<T, Sphere>;
	using SphereParams = SphereConcepts::ParamMapping;

	using SphereFactory = TFactory<SphereField, SphereParams>;
}
