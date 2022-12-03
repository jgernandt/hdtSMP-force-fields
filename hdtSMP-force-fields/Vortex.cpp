#include "pch.h"
#include "Vortex.h"

void jg::Vortex::actOn(btRigidBody& body) const
{
	if (!expired()) {

		//Doing the inclusion test in world space might be faster, 
		//since we could reuse the height vector for direction.

		btVector3 p = toLocal(body.getWorldTransform().getOrigin());

		if (includes(p)) {

			float F = getForce() / getRadius();
			btVector3 r_vec(p.x(), p.y(), 0.0f);
			float r = r_vec.norm();

			//norm of local_dir equals r (since they are perpendicular and the first is unit)
			btVector3 local_dir = btVector3(0.0f, 0.0f, 1.0f).cross(r_vec);
			btVector3 dir = Rotatable::toWorld(local_dir);

			if (hasDecay()) {
				F *= getDecay(r / getRadius());
			}

			if (fluctuating()) {
				float angle = std::copysign(std::acos(p.x() / r), p.y());
				F *= get2Freq(angle);
			}

			if (hasNoise()) {
				body.applyCentralForce(F * (dir + r * noiseVector()));
			}
			else {
				body.applyCentralForce(F * dir);
			}
		}
	}
}
