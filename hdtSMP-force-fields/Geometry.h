#pragma once
#include "Transformable.h"

namespace jg
{
	//Calculate bound of geometry to optimise inclusion misses?

	//A rectangular cuboid geometry centered on the origin
	class Cuboid : public Transformable
	{
	public:
		struct ParamMapping
		{
			Transformable::ParamMapping transformable;

			Skyrim::BSFixedString vBox{ "vBox" };

			template<typename T>
			void operator()(Cuboid& target, T* owner, const ParamMap& params) const
			{
				transformable(target, owner, params);

				if (auto it = params.find(vBox); it != params.end()) {
					target.m_dims.setValue(std::abs(it->second.v[0]), std::abs(it->second.v[1]), std::abs(it->second.v[2]));
				}
				else {
					assert(owner);
					target.m_dims.setValue(
						obj_traits<T>::radius(*owner),
						obj_traits<T>::radius(*owner), 
						obj_traits<T>::length(*owner));
				}
				target.m_maxDim = std::max({ target.m_dims[0], target.m_dims[1],target.m_dims[2] });

				_DMESSAGE("Box: (%f, %f, %f)", target.m_dims[0], target.m_dims[1], target.m_dims[2]);
			}

		private:
		};

	public:
		Cuboid() = default;
		~Cuboid() = default;

		float getCharacteristicSize() const { return m_maxDim; }

		//test if this geometry includes the local point p
		bool includes(const btVector3& p) const
		{
#if defined(BT_USE_SSE_IN_API) && defined(BT_USE_SSE)
			return (0xf == _mm_movemask_ps((__m128)_mm_cmple_ps(p.absolute().mVec128, m_dims.mVec128)));
#else
			btVector3 abs_p = p.absolute();
			return ((m_dims[2] <= abs_p[2]) && (m_dims[1] <= abs_p[1]) && (m_dims[0] <= abs_p[0]));
#endif
		}

		template<typename T>
		void update(T* owner, float dt) 
		{
			Transformable::update(owner, dt);
		}

	private:
		btVector3 m_dims;
		float m_maxDim;
	};

	//A geometry that has a concept of "radius"
	class Polar : public Transformable
	{
	public:
		struct ParamMapping
		{
			Transformable::ParamMapping transformable;

			Skyrim::BSFixedString fRadius{ "fRadius" };

			template<typename T>
			void operator()(Polar& target, T* owner, const ParamMap& params) const
			{
				transformable(target, owner, params);

				if (auto it = params.find(fRadius); it != params.end() && it->second.f >= 0.0f) {
					target.m_radius = it->second.f;
				}
				else {
					assert(owner);
					target.m_radius = obj_traits<T>::radius(*owner);
				}
				target.updateInternal();

				_DMESSAGE("Radius: %f", target.m_radius);
			}
		};

	public:
		Polar() = default;
		~Polar() = default;

		float getCharacteristicSize() const { return getRadius(); }

		float getRadius() const { return m_radius; }
		float getRadius2() const { return m_radius2; }

		//test if this geometry includes the local point p
		bool includes(const btVector3& p) const
		{
			return p.dot(p) <= getRadius2();
		}

		template<typename T>
		void update(T* owner, float dt)
		{
			Transformable::update(owner, dt);
			updateInternal();
		}

	private:
		void updateInternal()
		{
			//do we also update from the owner?
			m_radius2 = m_radius * m_radius;
		}

	private:
		float m_radius{ 0.0f };
		float m_radius2{ 0.0f };
	};

	//A spherical geometry centered on the origin
	//Pointless right now. Will we need it later?
	class Spherical : public Polar
	{
	public:
		Spherical() = default;
		~Spherical() = default;

		template<typename T>
		void update(T* owner, float dt)
		{
			Polar::update(owner, dt);
		}
	};

	//A cylindrical geometry along Z, centered on the origin
	class Cylindrical : public Polar
	{
	public:
		struct ParamMapping
		{
			Polar::ParamMapping polar;

			Skyrim::BSFixedString fLength{ "fLength" };

			template<typename T>
			void operator()(Cylindrical& target, T* owner, const ParamMap& params) const
			{
				polar(target, owner, params);

				if (auto it = params.find(fLength); it != params.end() && it->second.f >= 0.0f) {
					target.m_length = it->second.f;
				}
				else {
					assert(owner);
					target.m_length = obj_traits<T>::length(*owner);
				}
				_DMESSAGE("Length: %f", target.m_length);
			}
		};

	public:
		Cylindrical() = default;
		~Cylindrical() = default;

		//test if this geometry includes the local point p
		bool includes(const btVector3& p) const
		{
			return std::abs(p.z()) <= m_length && Polar::includes(btVector3(p.x(), p.y(), 0.0f));
		}

		template<typename T>
		void update(T* owner, float dt)
		{
			Polar::update(owner, dt);
		}

	private:
		//half length, actually
		float m_length{ 0.0f };
	};
}
