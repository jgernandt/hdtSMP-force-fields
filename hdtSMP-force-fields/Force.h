#pragma once
#include <cassert>
#include <cmath>
#include "BSFixedString.h"
#include "BulletAPI.h"
#include "Config.h"
#include "IForceField.h"
#include "Objects.h"
#include "utils.h"

namespace jg
{
	class Force
	{
	public:
		struct ParamMapping
		{
			Skyrim::BSFixedString fForce{ "fForce" };

			template<typename T>
			void operator()(Force& target, T* owner, const ParamMap& params) const
			{
				if (auto it = params.find(fForce); it != params.end()) {
					target.m_intensity = it->second.f;
				}
				else {
					assert(owner);
					target.m_intensity = obj_traits<T>::force(*owner);
				}

				_DMESSAGE("Force: %f", target.m_intensity);
			}
		};

	public:
		Force() = default;
		~Force() = default;

		float getForce() const { return g_config.getf(Config::FORCE_MULTIPLIER) * m_intensity; }

		template<typename T>
		void update(T* owner, float dt) {}

	protected:
		float m_intensity;
	};

	class Decaying
	{
	public:
		struct ParamMapping
		{
			Skyrim::BSFixedString fRange{ "fRange" };

			template<typename T>
			void operator()(Decaying& target, T* owner, const ParamMap& params) const
			{
				if (auto it = params.find(fRange); it != params.end() && it->second.f > 0.0f) {
					//interpret range as half-life, in some user-defined units of the geometry size
					target.m_decay = 0.6931472f / it->second.f;
				}
				else {
					target.m_decay = 0.0f;
				}

				_DMESSAGE("Decay: %f", target.m_decay);
			}
		};

	public:
		Decaying() = default;
		~Decaying() = default;

		float getDecay(float x) const { return std::exp(-m_decay * x); }
		bool hasDecay() const { return m_decay > 0.0f; }

		template<typename T>
		void update(T* owner, float dt) {}

	private:
		float m_decay;
	};


	class Noisy
	{
	public:
		struct ParamMapping
		{
			Skyrim::BSFixedString fNoise{ "fNoise" };

			template<typename T>
			void operator()(Noisy& target, T* owner, const ParamMap& params) const
			{
				if (auto it = params.find(fNoise); it != params.end() && it->second.f >= 0.0f) {
					target.m_noise = it->second.f;
				}
				else {
					target.m_noise = 0.0f;
				}

				_DMESSAGE("Noise: %f", target.m_noise);
			}
		};

	public:
		Noisy() = default;
		~Noisy() = default;

		bool hasNoise() const { return m_noise != 0.0f; }

		btVector3 noiseVector() const
		{
			if (m_noise != 0.0f) {
				return btVector3(randf(-m_noise, m_noise), randf(-m_noise, m_noise), randf(-m_noise, m_noise));
			}
			else {
				return btVector3(0.0f, 0.0f, 0.0f);
			}
		}
		
		template<typename T>
		void update(T* owner, float dt) {}

	private:
		float m_noise;
	};
}
