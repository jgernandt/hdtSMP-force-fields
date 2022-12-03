#pragma once
#include <atomic>
#include <cstdint>

namespace Skyrim
{
	class NiRefObject
	{
	public:
		NiRefObject();
		virtual ~NiRefObject();

		virtual void deleteThis();

		void IncRefCount()
		{
			m_refCount.fetch_add(1, std::memory_order_acq_rel);
		}
		void DecRefCount()
		{
			if (m_refCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
				deleteThis();
			}
		}

	protected:
		std::atomic<std::uint32_t> m_refCount;
		std::uint32_t pad0c;
	};
	static_assert(sizeof(NiRefObject) == 0x10);
}
