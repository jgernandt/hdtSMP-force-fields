#pragma once
#include "Concepts.h"
#include "TForceField.h"

namespace jg
{
	template<typename Geom>
	struct plane_traits
	{
		using concepts_type = TConcepts<Force, Directional<Geom>, Fluctuating<Expiring<Clock>>, Decaying, Noisy>;

		struct impl_type : concepts_type
		{
			void actOn(btRigidBody& body) const
			{
				if (!this->expired()) {

					btVector3 p = this->toLocal(body.getWorldTransform().getOrigin());

					if (this->includes(p)) {

						float F = this->getForce();

						if (this->hasDecay()) {
							F *= this->getDecay(p.norm() / this->getCharacteristicSize());
						}

						if (this->fluctuating()) {
							F *= this->get2Freq(p.dot(this->getLocalDir()));
						}

						btVector3 dir = this->getDirection();

						if (this->hasNoise()) {
							dir += this->noiseVector();
						}

						body.applyCentralForce(F * dir);
					}
				}
			}
		};

		template<typename T> using field_type = TField<T, impl_type>;
		using mapping_type = concepts_type::ParamMapping;
		using factory_type = TFactory<field_type, mapping_type>;
	};

	using BallFactory = plane_traits<Spherical>::factory_type;
	using BoxFactory = plane_traits<Cuboid>::factory_type;
	using RodFactory = plane_traits<Cylindrical>::factory_type;
}
