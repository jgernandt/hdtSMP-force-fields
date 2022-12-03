#pragma once
#include "Concepts.h"
#include "TForceField.h"

namespace jg
{
	using VortexConcepts = TConcepts<Force, Cylindrical, Fluctuating<Expiring<Clock>>, Decaying, Noisy>;

	struct Vortex : VortexConcepts
	{
		void actOn(btRigidBody& body) const;
	};

	template<typename T> using VortexField = TField<T, Vortex>;
	using VortexParams = VortexConcepts::ParamMapping;

	using VortexFactory = TFactory<VortexField, VortexParams>;
}
