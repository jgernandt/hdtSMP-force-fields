#pragma once
#include "Objects.h"
#include "BSFixedString.h"
#include "IForceField.h"
#include "utils.h"

namespace jg
{
	class Clock
	{
	public:
		struct ParamMapping
		{
			template<typename T>
			void operator()(Clock& target, T* owner, const ParamMap& params) const {}
		};

	public:
		Clock() = default;
		~Clock() = default;

		template<typename T>
		void update(T * owner, float dt)
		{
			if constexpr (obj_traits<T>::hasAge()) {
				assert(owner);
				m_time = obj_traits<T>::age(*owner);
			}
			else {
				m_time += dt;
			}
		}

		float getTime() const { return m_time; }

	protected:
		float m_time{ 0.0f };
	};

	//Clock decorators

	template<typename Base> requires std::is_base_of_v<Clock, Base>
	class Expiring : public Base
	{
	public:
		struct ParamMapping
		{
			Base::ParamMapping base;

			Skyrim::BSFixedString fDuration{ "fDuration" };

			template<typename T>
			void operator()(Expiring& target, T* owner, const ParamMap& params) const
			{
				base(target, owner, params);

				if (auto it = params.find(fDuration); it != params.end()) {
					target.m_duration = it->second.f;
				}
				else {
					assert(owner);
					target.m_duration = obj_traits<T>::duration(*owner);
				}

				_DMESSAGE("Duration: %f", target.getDuration());
			}
		};

	public:
		Expiring() = default;
		~Expiring() = default;

		float getDuration() const { return m_duration; }
		bool expired() const { return m_expired; }

		template<typename T>
		void update(T* owner, float dt)
		{
			Base::update(owner, dt);

			assert(owner);
			m_expired = obj_traits<T>::expired(*owner) 
				|| (m_duration > 0.0f && this->m_time >= m_duration);
		}

	private:
		float m_duration{ -1.0f };
		bool m_expired{ false };
	};

	template<typename Base> requires std::is_base_of_v<Clock, Base>
	class Fluctuating : public Base
	{
	public:
		struct ParamMapping
		{
			Base::ParamMapping base;

			Skyrim::BSFixedString fAmplitude{ "fAmplitude" };
			Skyrim::BSFixedString fFrequency{ "fFrequency" };
			Skyrim::BSFixedString fSpeed{ "fSpeed" };

			template<typename T>
			void operator()(Fluctuating& target, T* owner, const ParamMap& params) const
			{
				base(target, owner, params);

				if (auto it = params.find(fAmplitude); it != params.end() && it->second.f >= 0.0f) {
					target.m_amplitude = it->second.f;
					target.m_halfAmp = 0.5f * target.m_amplitude;
				}
				_DMESSAGE("Amplitude: %f", target.m_amplitude);

				if (auto it = params.find(fFrequency); it != params.end() && it->second.f > 0.0f) {
					target.m_frequency = it->second.f;
					target.m_angFreq = 2.0f * PI * target.m_frequency;
				}
				target.m_angFreq2 = target.m_angFreq * randf(0.15f, 0.3f);
				_DMESSAGE("Frequency: %f", target.m_frequency);

				if (auto it = params.find(fSpeed); it != params.end() && it->second.f > 0.0f) {
					target.m_speed = it->second.f;
				}
				_DMESSAGE("Speed: %f", target.m_speed);
			}
		};

	public:
		Fluctuating() = default;
		~Fluctuating() = default;

		bool fluctuating() const { return m_amplitude > 0.0f; }

		float get1Freq(float x) const
		{
			float a = x / m_speed - this->getTime();
			return 1.0f + m_amplitude * std::cos(m_angFreq * a);
		}

		float get2Freq(float x) const
		{
			float a = x / m_speed - this->getTime();
			return 1.0f + m_halfAmp * (std::cos(m_angFreq * a) + std::cos(m_angFreq2 * a));
		}

		template<typename T>
		void update(T* owner, float dt)
		{
			Base::update(owner, dt);
		}

	private:
		float m_amplitude{ 0.0f };
		float m_frequency{ 1.0f };
		float m_speed{ 1.0f };

		float m_angFreq{ 2.0f * PI };
		float m_halfAmp{ 0.0f };

		float m_angFreq2{ 2.0f * PI };
	};
}
