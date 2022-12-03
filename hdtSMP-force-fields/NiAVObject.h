#pragma once
#include "NiMath.h"
#include "NiObjectNET.h"

namespace Skyrim
{
	class NiNode;
	class ObjectReference;

	class NiAVObject : public NiObjectNET
	{
	public:
		virtual void NiAVObject_25();
		virtual void NiAVObject_26();
		virtual void NiAVObject_27();
		virtual void NiAVObject_28();
		virtual void NiAVObject_29();
		virtual NiAVObject* getObjectByName(const BSFixedString& name);
		virtual void NiAVObject_2b();
		virtual void NiAVObject_2c();
		virtual void NiAVObject_2d();
		virtual void NiAVObject_2e();
		virtual void NiAVObject_2f();
		virtual void NiAVObject_30();
		virtual void NiAVObject_31();
		virtual void NiAVObject_32();
		virtual void NiAVObject_33();
		virtual void NiAVObject_34();

		NiNode* getParent() const { return get_member<NiNode*>(this, offsetof_parent); }
		const NiTransform& getWorldTransform() const { return get_member<NiTransform>(this, offsetof_worldTransform); }
		ObjectReference* getOwner() const { return get_member<ObjectReference*>(this, offsetof_owner); }

	private:
		constexpr static std::size_t offsetof_parent = sizeof(NiObjectNET);
		constexpr static std::size_t offsetof_worldTransform = sizeof(NiObjectNET) + 0x4c;
		constexpr static std::size_t offsetof_owner = sizeof(NiObjectNET) + 0xc8;

		char data[0xe0];
	};
	static_assert(sizeof(NiAVObject) == 0x110);
}
