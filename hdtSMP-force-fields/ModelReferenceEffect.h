#pragma once
#include "BSFixedString.h"
#include "NiObject.h"
#include "get_member.h"
#include "ni_ptr.h"

namespace Skyrim
{
	class Cell;
	class NiAVObject;
	class ObjectReference;

	class BSTempEffect : public NiObject
	{
	public:
		virtual void BSTempEffect_25();
		virtual void BSTempEffect_26();
		virtual void BSTempEffect_27();
		virtual void BSTempEffect_28();

		virtual NiAVObject* get3D() const;

		virtual void BSTempEffect_2a();
		virtual void BSTempEffect_2b();

		virtual std::uint32_t getType() const;

		virtual void BSTempEffect_2d();
		virtual void BSTempEffect_2e();
		virtual void BSTempEffect_2f();
		virtual void BSTempEffect_30();
		virtual void BSTempEffect_31();
		virtual void BSTempEffect_32();
		virtual void BSTempEffect_33();
		virtual void BSTempEffect_34();
		virtual void BSTempEffect_35();

		float getAge() const { return get_member<float>(this, offsetof_age); }

	private:
		constexpr static std::size_t offsetof_age = sizeof(NiObject) + 0x10;

		char data[0x20];
	};
	static_assert(sizeof(BSTempEffect) == 0x30);

	class ReferenceEffect : public BSTempEffect
	{
	public:
		virtual void ReferenceEffect_36();
		virtual void ReferenceEffect_37();
		virtual void ReferenceEffect_38();
		virtual void ReferenceEffect_39();
		virtual void ReferenceEffect_3a();
		virtual void ReferenceEffect_3b();
		virtual void ReferenceEffect_3c();

		virtual bool isAttached();
		virtual void detachImpl();

		bool getFinished() const { return get_member<bool>(this, offsetof_finished); }
		ni_ptr<ObjectReference> getTarget() const;

	private:
		constexpr static std::size_t offsetof_target = sizeof(BSTempEffect) + 8;
		constexpr static std::size_t offsetof_finished = sizeof(BSTempEffect) + 0x10;

		char data[0x18];
	};
	static_assert(sizeof(ReferenceEffect) == 0x48);


	class ModelReferenceEffect : public ReferenceEffect
	{
	private:
		constexpr static std::size_t offsetof_ModelReferenceEffect = 0x68;

		char data[0x90];
	};
	static_assert(sizeof(ModelReferenceEffect) == 0xD8);
}
