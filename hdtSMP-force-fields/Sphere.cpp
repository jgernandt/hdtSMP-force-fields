#include "pch.h"
#include "Sphere.h"

void jg::Sphere::actOn(btRigidBody& body) const
{
	if (!expired()) {

		//Doing the inclusion test in world space might be faster, 
		//since we could reuse the radial vector for direction.

		btVector3 p = toLocal(body.getWorldTransform().getOrigin());

		if (includes(p)) {

			float r = p.norm();

			float F = getForce();

			if (hasDecay()) {
				F *= getDecay(r / getRadius());
			}

			if (fluctuating()) {
				F *= get2Freq(r);
			}

			btVector3 dir = Rotatable::toWorld(p / r);

			if (hasNoise()) {
				dir += noiseVector();
			}

			body.applyCentralForce(F * dir);
		}
	}
}
