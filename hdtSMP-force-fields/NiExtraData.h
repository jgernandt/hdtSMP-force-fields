#pragma once
#include "BSFixedString.h"
#include "NiObject.h"

namespace Skyrim
{
	class NiExtraData : public NiObject
	{
	public:
		virtual void NiExtraData_25();
		virtual void NiExtraData_26();

		const char* getName() const { return m_name.c_str(); }

	private:
		BSFixedString m_name;
	};
	static_assert(sizeof(NiExtraData) == 0x18);
}
