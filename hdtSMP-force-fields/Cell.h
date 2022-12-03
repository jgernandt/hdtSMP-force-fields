#pragma once
#include "Form.h"

namespace Skyrim
{
	class Cell : public Form
	{
	public:
		bool isAttached() const { return get_member<std::uint8_t>(this, offsetof_cellState) == 7; }

	private:
		constexpr static std::size_t offsetof_Cell = 0x30;
		constexpr static std::size_t offsetof_cellState = offsetof_Cell + 0x14;
	};
}
