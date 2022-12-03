#include "pch.h"
#include "NiObjectNET.h"
#include "NiExtraData.h"

Skyrim::NiExtraData* Skyrim::NiObjectNET::findExtraData(const BSFixedString& name) const
{
	for (std::uint_fast16_t i = 0; i < m_extraDataLen; i++) {
		if (m_extraData[i] && m_extraData[i]->getName() == name.c_str()) {
			return m_extraData[i];
		}
	}
	return nullptr;
}
