#include "pch.h"
#include "Cylinder.h"

void jg::Cylinder::actOn(btRigidBody& body) const
{
	if (!expired()) {

		//Doing the inclusion test in world space might be faster, 
		//since we could reuse the radial vector for direction.

		btVector3 p = toLocal(body.getWorldTransform().getOrigin());

		if (includes(p)) {

			btVector3 r_vec(p.x(), p.y(), 0.0f);
			float r = r_vec.norm();

			float F = getForce();

			if (hasDecay()) {
				//Only decay radially? Is that what you would expect?
				F *= getDecay(r / getRadius());
			}

			if (fluctuating()) {
				F *= get2Freq(r);
			}

			btVector3 dir = Rotatable::toWorld(r_vec / r);

			if (hasNoise()) {
				dir += noiseVector();
			}
			
			body.applyCentralForce(F * dir);
		}
	}
}
