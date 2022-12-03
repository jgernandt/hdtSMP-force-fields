#pragma once
#include "Concepts.h"
#include "TForceField.h"

namespace jg
{
	using CylinderConcepts = TConcepts<Force, Cylindrical, Fluctuating<Expiring<Clock>>, Decaying, Noisy>;

	struct Cylinder : CylinderConcepts
	{
		void actOn(btRigidBody& body) const;
	};

	template<typename T> using CylinderField = TField<T, Cylinder>;
	using CylinderParams = CylinderConcepts::ParamMapping;

	using CylinderFactory = TFactory<CylinderField, CylinderParams>;
}
