#pragma once
#include "BSFixedString.h"
#include "NiObject.h"
#include "ni_ptr.h"

namespace Skyrim
{
	class NiExtraData;
	class NiTimeController;

	class NiObjectNET : public NiObject
	{
	public:
		const char* getName() const { return m_name.c_str(); }
		NiExtraData* findExtraData(const BSFixedString& name) const;

	protected:
		BSFixedString		m_name;				// 10
		ni_ptr<NiTimeController> m_controller;	// 18
		NiExtraData**		m_extraData;		// 20
		std::uint16_t		m_extraDataLen;		// 28
		std::uint16_t		m_extraDataCapacity;// 2A
		std::uint32_t		pad2C;				// 2C
	};
	static_assert(sizeof(NiObjectNET) == 0x30);
}
