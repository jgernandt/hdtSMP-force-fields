#pragma once
#include "IForceField.h"
#include "Objects.h"
#include "utils.h"

namespace jg
{
	template<typename... Concepts>
	struct ParamMapping : Concepts::ParamMapping...
	{};

	template<typename... Concepts>
	class TConcepts : public Concepts...
	{
	public:
		using ParamMapping = jg::ParamMapping<Concepts...>;

	public:
		TConcepts() = default;
		~TConcepts() = default;

		template<typename T>
		void mapParams(T* owner, const ParamMap& params, const ParamMapping& mapping)
		{
			int dummy[sizeof...(Concepts)] = { (static_cast<const Concepts::ParamMapping&>(mapping)(*this, owner, params), 0)... };
		}

		template<typename T>
		void update(T* owner, float dt)
		{
			int dummy[sizeof...(Concepts)] = { (static_cast<Concepts&>(*this).update(owner, dt), 0)... };
		}
	};

	
	template<typename T, typename Impl>
	class TField : public IForceField
	{
	public:
		template<typename PM>
		TField(T* owner, const ParamMap& params, const PM& pm) :
			m_owner{ owner }
		{
			_DMESSAGE("%s<%s>(%p)", typeid(Impl).name(), typeid(T).name(), this);
			gLog.Indent();
			m_impl.mapParams(owner, params, pm);
			gLog.Outdent();
		}

		virtual ~TField() override
		{
			_DMESSAGE("~%s<%s>(%p)", typeid(Impl).name(), typeid(T).name(), this);
		}

		virtual void actOn(btRigidBody& body) const override
		{
			m_impl.actOn(body);
		}

		virtual bool update(float dt) override
		{
			assert(m_owner);

			if (obj_traits<T>::isAttached(*m_owner)) {
				m_impl.update(m_owner, dt);
				return true;
			}
			else {
				return false;
			}
		}

		Impl m_impl;
		T* m_owner;
	};

	template<template<typename> typename FieldType, typename ParamMapping>
	class TFactory : public IForceFieldFactory
	{
	public:
		TFactory() = default;
		virtual ~TFactory() override = default;

		[[nodiscard]] virtual std::unique_ptr<IForceField> create(Skyrim::ModelReferenceEffect* owner, const ParamMap& params) override
		{
			return createImpl(owner, params);
		}
		[[nodiscard]] virtual std::unique_ptr<IForceField> create(Skyrim::ObjectReference* owner, const ParamMap& params) override
		{
			assert(owner);

			switch (owner->getFormType()) {
			case Skyrim::Hazard::FORM_TYPE:
				return createImpl(static_cast<Skyrim::Hazard*>(owner), params);
			case Skyrim::ConeProjectile::FORM_TYPE:
				return createImpl(static_cast<Skyrim::ConeProjectile*>(owner), params);
			case Skyrim::FormType::REF_MISSILE:
			case Skyrim::FormType::REF_ARROW:
			case Skyrim::FormType::REF_GRENADE:
			case Skyrim::FormType::REF_BEAM:
			case Skyrim::FormType::REF_FLAME:
			case Skyrim::FormType::REF_BARRIER:
				return createImpl(static_cast<Skyrim::Projectile*>(owner), params);
			default:
				if (auto exp = owner->asExplosion()) {
					return createImpl(exp, params);
				}
				else {
					return createImpl(owner, params);
				}
			}
		}

		template<typename T>
		std::unique_ptr<FieldType<T>> createImpl(T* owner, const ParamMap& params)
		{
			return std::make_unique<FieldType<T>>(owner, params, paramMapping);
		}

		ParamMapping paramMapping;
	};
}
