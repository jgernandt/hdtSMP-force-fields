#pragma once
#include <type_traits>
#include "NiRefObject.h"
#include "get_member.h"

namespace Skyrim
{
	class NiNode;
	class NiRTTI;

	class NiObject : public NiRefObject
	{
	public:
		virtual const NiRTTI* GetRTTI() const;
		virtual NiNode* asNiNode() { return nullptr; }
		virtual void NiObject_04();
		virtual void NiObject_05();
		virtual void NiObject_06();
		virtual void NiObject_07();
		virtual void NiObject_08();
		virtual void NiObject_09();
		virtual void NiObject_0a();
		virtual void NiObject_0b();
		virtual void NiObject_0c();
		virtual void NiObject_0d();
		virtual void NiObject_0e();
		virtual void NiObject_0f();
		virtual void NiObject_10();
		virtual void NiObject_11();
		virtual void NiObject_12();
		virtual void NiObject_13();
		virtual void NiObject_14();
		virtual void NiObject_15();
		virtual void NiObject_16();
		virtual void NiObject_17();
		virtual void NiObject_18();
		virtual void NiObject_19();
		virtual void NiObject_1a();
		virtual void NiObject_1b();
		virtual void NiObject_1c();
		virtual void NiObject_1d();
		virtual void NiObject_1e();
		virtual void NiObject_1f();
		virtual void NiObject_20();
		virtual void NiObject_21();
		virtual void NiObject_22();
		virtual void NiObject_23();
		virtual void NiObject_24();
	};
	static_assert(sizeof(NiObject) == 0x10);
}

template<typename T> requires std::is_base_of_v<Skyrim::NiObject, T>
inline bool is_ni_type(Skyrim::NiObject* obj)
{
	return obj && obj->GetRTTI() == T::NI_TYPE;
}

template<typename T> requires std::is_base_of_v<Skyrim::NiObject, T>
inline bool is_ni_type(const Skyrim::NiObject& obj)
{
	return obj.GetRTTI() == T::NI_TYPE;
}
