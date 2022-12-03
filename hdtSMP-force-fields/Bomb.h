#pragma once
#include "Concepts.h"
#include "TForceField.h"

namespace jg
{
	using BombConcepts = TConcepts<Force, Polar, Clock>;

	struct BombParams : BombConcepts::ParamMapping
	{
		Skyrim::BSFixedString fSpeed{ "fSpeed" };
	};

	struct Bomb : BombConcepts
	{
		template<typename T>
		void mapParams(T* owner, const ParamMap& params, const BombParams& mapping)
		{
			BombConcepts::mapParams(owner, params, mapping);

			//m_speed = 20000.0f;//the speed of sound in game units (too high to look good?)
			m_speed = 5000.0f;
			_DMESSAGE("Speed: %f", m_speed);

			m_Rmax = 2.0f * getRadius();
			m_duration = m_Rmax / m_speed;
			m_recSpeed = 1.0f / m_speed;
			m_rec2Speed = m_recSpeed / 2.0f;
		}

		template<typename T>
		void update(T* owner, float dt)
		{
			if (m_lastTime < m_duration) {
				m_lastTime = getTime();

				BombConcepts::update(owner, dt);

				float waveFront = m_speed * getTime();
				m_waveFrontSq = waveFront * waveFront;

				float waveBack = std::max(2.0f * m_speed * m_lastTime - m_Rmax, 0.0f);
				m_lastWaveBackSq = waveBack * waveBack;

				m_c1 = getForce() / ((getTime() - m_lastTime) * m_Rmax);
			}
		}

		void actOn(btRigidBody& body) const;

	private:
		//parameters
		float m_speed;

		//derived params
		float m_Rmax;
		float m_duration;
		float m_recSpeed;
		float m_rec2Speed;

		//variables
		float m_lastTime{ 0.0f };
		float m_waveFrontSq{ 0.0f };
		float m_lastWaveBackSq{ 0.0f };
		float m_c1{ 0.0f };
	};

	template<typename T> using BombField = TField<T, Bomb>;
	using BombFactory = TFactory<BombField, BombParams>;
}
