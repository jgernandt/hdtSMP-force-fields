#pragma once
#include "Form.h"
#include "NiMath.h"
#include "NiRefObject.h"
#include "var_size.h"

template<typename T> class ni_ptr;

namespace Skyrim
{
	class BSHandleRefObject : public NiRefObject
	{
	private:
		constexpr static std::uint32_t MASK_REF_COUNT = 0x3ff;

	public:
		void DecRefCount()
		{
			if ((m_refCount.fetch_sub(1, std::memory_order_acq_rel) & MASK_REF_COUNT) == 1) {
				deleteThis();
			}
		}
	};
	static_assert(sizeof(NiRefObject) == 0x10);

	class Cell;
	class Explosion;
	class NiAVObject;
	class Projectile;

	class ObjectReference :
		public Form,
		public BSHandleRefObject
	{
	public:
		constexpr static FormType FORM_TYPE = FormType::REF_OBJECT;

	public:
		virtual void ObjectReference_3b();
		virtual void ObjectReference_3c();
		virtual void ObjectReference_3d();
		virtual void ObjectReference_3e();
		virtual void ObjectReference_3f();
		virtual void ObjectReference_40();
		virtual void ObjectReference_41();
		virtual void ObjectReference_42();
		virtual void ObjectReference_43();
		virtual void ObjectReference_44();
		virtual void ObjectReference_45();
		virtual void ObjectReference_46();
		virtual void ObjectReference_47();
		virtual void ObjectReference_48();
		virtual void ObjectReference_49();
		virtual void ObjectReference_4a();
		virtual void ObjectReference_4b();
		virtual void ObjectReference_4c();
		virtual void ObjectReference_4d();
		virtual void ObjectReference_4e();
		virtual void ObjectReference_4f();
		virtual void ObjectReference_50();
		virtual void ObjectReference_51();
		virtual void ObjectReference_52();
		virtual void ObjectReference_53();
		virtual void ObjectReference_54();
		virtual void ObjectReference_55();
		virtual void ObjectReference_56();
		virtual void ObjectReference_57();
		virtual void ObjectReference_58();
		virtual void ObjectReference_59();
		virtual void ObjectReference_5a();
		virtual void ObjectReference_5b();
		virtual void ObjectReference_5c();
		virtual void ObjectReference_5d();
		virtual void ObjectReference_5e();
		virtual void ObjectReference_5f();
		virtual void ObjectReference_60();
		virtual void ObjectReference_61();
		virtual void ObjectReference_62();
		virtual void ObjectReference_63();
		virtual void ObjectReference_64();
		virtual void ObjectReference_65();
		virtual void ObjectReference_66();
		virtual void ObjectReference_67();
		virtual void ObjectReference_68();
		virtual void ObjectReference_69();

		virtual NiAVObject* load3D(bool bkgnd);
		virtual void release3D();

		virtual void ObjectReference_6c();
		virtual void ObjectReference_6d();
		virtual void ObjectReference_6e();
		virtual void ObjectReference_6f();

		virtual NiAVObject* get3D() const;

		virtual void ObjectReference_71();
		virtual void ObjectReference_72();
		virtual void ObjectReference_73();
		virtual void ObjectReference_74();
		virtual void ObjectReference_75();
		virtual void ObjectReference_76();
		virtual void ObjectReference_77();
		virtual void ObjectReference_78();
		virtual void ObjectReference_79();
		virtual void ObjectReference_7a();
		virtual void ObjectReference_7b();
		virtual void ObjectReference_7c();
		virtual void ObjectReference_7d();
		virtual void ObjectReference_7e();
		virtual void ObjectReference_7f();
		virtual void ObjectReference_80();
		virtual void ObjectReference_81();
		virtual void ObjectReference_82();
		virtual void ObjectReference_83();
		virtual void ObjectReference_84();
		virtual void ObjectReference_85();
		virtual void ObjectReference_86();
		virtual void ObjectReference_87();
		virtual void ObjectReference_88();
		virtual void ObjectReference_89();
		virtual void ObjectReference_8a();
		virtual void ObjectReference_8b();
		virtual void ObjectReference_8c();
		virtual void ObjectReference_8d();

		virtual Explosion* asExplosion() { return nullptr; }
		virtual Projectile* asProjectile() { return nullptr; }

		virtual void ObjectReference_90();
		virtual void ObjectReference_91();
		virtual void ObjectReference_92();
		virtual void ObjectReference_93();
		virtual void ObjectReference_94();
		virtual void ObjectReference_95();
		virtual void ObjectReference_96();
		virtual void ObjectReference_97();
		virtual void ObjectReference_98();
		virtual void ObjectReference_99();
		virtual void ObjectReference_9a();
		virtual void ObjectReference_9b();
		virtual void ObjectReference_9c();
		virtual void ObjectReference_9d();
		virtual void ObjectReference_9e();
		virtual void ObjectReference_9f();
		virtual void ObjectReference_a0();
		virtual void ObjectReference_a1();

		Form* getBaseForm() const { return get_member<Form*>(this, offsetof_ObjectReference); }

		template<typename T> requires std::is_base_of_v<Form, T>
		T* getBaseFormAs() const
		{
			if (auto base = getBaseForm(); base && base->getFormType() == T::FORM_TYPE) {
				return static_cast<T*>(base);
			}
			return nullptr;
		}

		Cell* getParentCell() const { return get_member<Cell*>(this, offsetof_parentCell); }

	private:
		constexpr static std::size_t offsetof_ObjectReference = 0x40;
		constexpr static std::size_t offsetof_parentCell = offsetof_ObjectReference + 0x20;

	protected:
		//variable size
		static var_offset sizeof_ObjectReference;
	};

	using LookupRefByHandle_t = bool (*)(const std::uint32_t&, ni_ptr<ObjectReference>&);
}
