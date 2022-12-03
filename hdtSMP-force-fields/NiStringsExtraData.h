#pragma once
#include "NiExtraData.h"

namespace Skyrim
{
	class NiStringsExtraData : public NiExtraData
	{
	public:
		constexpr static obj_ptr<NiRTTI, AddressTable::NiRTTI_NiStringsExtraData> NI_TYPE{};

		class const_iterator
		{
		public:
			const_iterator(const char** data) : m_data{ data } {}
			const_iterator(const const_iterator&) = default;

			const char* operator*() const { return *m_data; }
			const char* const* operator->() const { return m_data; }

			const_iterator& operator++()
			{
				m_data++;
				return *this;
			}

			const_iterator operator++(int)
			{
				const_iterator tmp = *this;
				++*this;
				return tmp;
			}

			friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) = default;

		protected:
			const char** m_data;
		};

		class iterator : public const_iterator
		{
		public:
			iterator(const char** data) : const_iterator(data) {}
			iterator(const iterator&) = default;

			const char*& operator*() { return *m_data; }
			const char** operator->() { return m_data; }

			iterator& operator++()
			{
				const_iterator::operator++();
				return *this;
			}

			iterator operator++(int)
			{
				iterator tmp = *this;
				const_iterator::operator++();
				return tmp;
			}
		};

		iterator begin() { return iterator(m_data); }
		const_iterator begin() const { return const_iterator(m_data); }

		iterator end() { return iterator(m_data + m_size); }
		const_iterator end() const { return const_iterator(m_data + m_size); }

		bool empty() const { return m_size == 0; }
		std::uint32_t size() const { return m_size; }

	private:
		std::uint32_t m_size;
		std::uint32_t pad;
		const char** m_data;
	};
	static_assert(sizeof(NiStringsExtraData) == 0x28);
}
