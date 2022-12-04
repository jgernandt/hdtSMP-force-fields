#include "pch.h"
#include "memory.h"
#include "relocation.h"

namespace Skyrim
{
	using MemoryManager__Allocate_t = void* (*)(MemoryManager*, std::size_t, std::size_t, bool);
	using MemoryManager__Free_t = void (*)(MemoryManager*, void*, bool);
	using MemoryManager__Get_t = MemoryManager* (*)();

	void* alloc(std::size_t count, std::size_t al)
	{
		fcn_ptr<MemoryManager__Get_t, AddressTable::MemoryManager__Get> MemoryManager__Get;
		fcn_ptr<MemoryManager__Allocate_t, AddressTable::MemoryManager__Allocate> MemoryManager__Allocate;

		if (void* ptr = MemoryManager__Allocate(MemoryManager__Get(), count, al, al > __STDCPP_DEFAULT_NEW_ALIGNMENT__)) {
			return ptr;
		}
		throw std::bad_alloc();
	}

	void free(void* ptr, std::size_t al)
	{
		fcn_ptr<MemoryManager__Get_t, AddressTable::MemoryManager__Get> MemoryManager__Get;
		fcn_ptr<MemoryManager__Free_t, AddressTable::MemoryManager__Free> MemoryManager__Free;

		MemoryManager__Free(MemoryManager__Get(), ptr, al > __STDCPP_DEFAULT_NEW_ALIGNMENT__);
	}
}
