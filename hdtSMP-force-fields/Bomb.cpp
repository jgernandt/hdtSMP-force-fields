#include "pch.h"
#include "Bomb.h"

void jg::Bomb::actOn(btRigidBody& body) const
{
	//At time = hitTime, a pressure wave should travel at high speed from origin to R.
	//The speed of the front is too high to use an instantaneous value for a whole frame.
	//Instead we integrate the force and average over the frame.
	//We use a saw wave front f(r, t) = f_0 * (1 + r / R - 2vt / R)

	if (m_lastTime < m_duration) {

		btVector3 offset = body.getWorldTransform().getOrigin() - getPosition();
		auto r2 = offset.dot(offset);

		//Ignore if outside the wave
		if (r2 < m_waveFrontSq && r2 > m_lastWaveBackSq) {
			float r = std::sqrt(r2);
			//the wave hits r at t = r / v
			float t1 = std::max(r * m_recSpeed, m_lastTime);
			//the wave passes r at t = (r + R) / 2v
			float t2 = std::min({ (r + m_Rmax) * m_rec2Speed, getTime(), m_duration });
			if (t1 < t2) {
				//F(r) * delta t = integral(f(r, t) dt) from t1 to t2
				body.applyCentralForce((m_c1 / r * ((r + m_Rmax) * (t2 - t1) - m_speed * (t2 * t2 - t1 * t1))) * offset);
			}
		}
	}
}
