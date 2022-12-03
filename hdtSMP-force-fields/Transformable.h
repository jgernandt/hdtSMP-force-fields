#pragma once
#include <cassert>
#include "BulletAPI.h"
#include "Objects.h"
#include "BSFixedString.h"
#include "IForceField.h"
#include "NiAVObject.h"

namespace jg
{
	class Translatable
	{
	public:
		struct ParamMapping
		{
			Skyrim::BSFixedString vTranslation{ "vTranslation" };

			template<typename T>
			void operator()(Translatable& target, T* owner, const ParamMap& params) const
			{
				if (auto it = params.find(vTranslation); it != params.end()) {
					target.m_translation.setValue(it->second.v[0], it->second.v[1], it->second.v[2]);
				}
				else {
					target.m_translation.setValue(0.0f, 0.0f, 0.0f);
				}
				target.update(owner, 0.0f);

				_DMESSAGE("Translation: (%f, %f, %f)", target.m_translation[0], target.m_translation[1], target.m_translation[2]);
			}

		private:
		};

	public:
		Translatable() = default;
		~Translatable() = default;

		const btVector3& getPosition() const { return m_position; }

		//transform a point in world space to local space
		btVector3 toLocal(const btVector3& p) const { return p - m_position; }

		//transform a point in world space to local space
		btVector3 toWorld(const btVector3& p) const { return p + m_position; }

		template<typename T>
		void update(T* owner, float dt)
		{
			assert(owner);
			m_position = obj_traits<T>::translate(*owner, m_translation);
		}

	protected:
		//our local translation
		btVector3 m_translation;

		//our current world space position
		btVector3 m_position;
	};

	class Rotatable
	{
	public:
		struct ParamMapping
		{
			Skyrim::BSFixedString qRotation{ "qRotation" };

			template<typename T>
			void operator()(Rotatable& target, T* owner, const ParamMap& params) const
			{
				if (auto it = params.find(qRotation); it != params.end()) {
					btQuaternion rot(it->second.v[1], it->second.v[2], it->second.v[3], it->second.v[0]);
					rot.safeNormalize();
					target.m_rotation.setRotation(rot);
				}
				else {
					target.m_rotation.setIdentity();
				}
				target.update(owner, 0.0f);

				_DMESSAGE("Rotation:");
				_DMESSAGE("\t[%f, %f, %f]",
					target.m_rotation.getRow(0)[0], target.m_rotation.getRow(0)[1], target.m_rotation.getRow(0)[2]);
				_DMESSAGE("\t[%f, %f, %f]",
					target.m_rotation.getRow(1)[0], target.m_rotation.getRow(1)[1], target.m_rotation.getRow(1)[2]);
				_DMESSAGE("\t[%f, %f, %f]",
					target.m_rotation.getRow(2)[0], target.m_rotation.getRow(2)[1], target.m_rotation.getRow(2)[2]);
			}

		private:
		};

	public:
		Rotatable() = default;
		~Rotatable() = default;

		const btMatrix3x3& getOrientation() const { return m_orientation; }

		//transform a point in world space to local space
		btVector3 toLocal(const btVector3& p) const { return m_orientationInv * p; }

		//transform a point in local space to world space
		btVector3 toWorld(const btVector3& p) const { return m_orientation * p; }

		template<typename T>
		void update(T* owner, float dt)
		{
			assert(owner);
			m_orientation = obj_traits<T>::rotate(*owner, m_rotation);
			m_orientationInv = m_orientation.transpose();
		}

	protected:
		//our local transform
		btMatrix3x3 m_rotation;

		//our current world space transform
		btMatrix3x3 m_orientation;
		btMatrix3x3 m_orientationInv;
	};

	class Scalable
	{
	public:
		struct ParamMapping
		{
			Skyrim::BSFixedString fScale{ "fScale" };

			template<typename T>
			void operator()(Scalable& target, T* owner, const ParamMap& params) const
			{
				if (auto it = params.find(fScale); it != params.end() && it->second.f > 0.0f) {
					target.m_scale = it->second.f;
				}
				else {
					target.m_scale = 1.0f;
				}
				target.update(owner, 0.0f);
				_DMESSAGE("Scale: %f", target.m_scale);
			}

		private:
		};

	public:
		Scalable() = default;
		~Scalable() = default;

		float getScale() const { return m_size; }

		//transform a point in world space to local space
		btVector3 toLocal(const btVector3& p) const { return p / m_size; }

		//transform a point in world space to local space
		btVector3 toWorld(const btVector3& p) const { return p * m_size; }

		template<typename T>
		void update(T* owner, float dt)
		{
			assert(owner);
			m_size = obj_traits<T>::scale(*owner, m_scale);
		}

	protected:
		//our local transform
		float m_scale;

		//our current world space transform
		float m_size;
	};

	class Transformable :
		public Translatable,
		public Rotatable,
		public Scalable
	{
	public:
		struct ParamMapping
		{
			Translatable::ParamMapping translatable;
			Rotatable::ParamMapping rotatable;
			Scalable::ParamMapping scalable;

			template<typename T>
			void operator()(Transformable& target, T* owner, const ParamMap& params) const
			{
				translatable(target, owner, params);
				rotatable(target, owner, params);
				scalable(target, owner, params);
			}

		private:
		};

	public:
		Transformable() = default;
		~Transformable() = default;

		//transform a point in world space to local space
		btVector3 toLocal(const btVector3& p) const 
		{ 
			return Scalable::toLocal(Rotatable::toLocal(Translatable::toLocal(p)));
		}

		//transform a point in local space to world space
		btVector3 toWorld(const btVector3& p) const
		{
			return Translatable::toWorld(Rotatable::toWorld(Scalable::toWorld(p)));
		}

		template<typename T>
		void update(T* owner, float dt)
		{
			Translatable::update(owner, dt);
			Rotatable::update(owner, dt);
			Scalable::update(owner, dt);
		}
	};

	//Decorators

	template<typename Base> requires std::is_base_of_v<Transformable, Base>
	class Directional : public Base
	{
	public:
		struct ParamMapping
		{
			Base::ParamMapping base;

			Skyrim::BSFixedString vDirection{ "vDirection" };

			template<typename T>
			void operator()(Directional& target, T* owner, const ParamMap& params) const
			{
				base(target, owner, params);

				if (auto it = params.find(vDirection); it != params.end()) {
					target.m_axis.setValue(it->second.v[0], it->second.v[1], it->second.v[2]);
					target.m_axis.normalize();
				}
				else {
					target.m_axis.setValue(1.0f, 0.0f, 0.0f);
				}
				target.updateInternal();
				_DMESSAGE("Direction: (%f, %f, %f)", target.m_axis[0], target.m_axis[1], target.m_axis[2]);
			}
		};

	public:
		Directional() = default;
		~Directional() = default;

		template<typename T>
		void update(T* owner, float dt)
		{
			Base::update(owner, dt);
			updateInternal();
		}

		const btVector3& getLocalDir() const { return m_axis; }
		const btVector3& getDirection() const { return m_direction; }

	private:
		void updateInternal()
		{
			m_direction = this->getOrientation() * m_axis;
		}

	private:
		//orientation in local space
		btVector3 m_axis;

		//current orientation in world space
		btVector3 m_direction;
	};
}
