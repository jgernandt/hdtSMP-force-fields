#include "pch.h"
#include "memory.h"
#include "relocation.h"

namespace Skyrim
{
	using MemoryManager__Allocate_t = void* (*)(MemoryManager*, std::size_t, std::size_t, bool);
	using MemoryManager__Free_t = void (*)(MemoryManager*, void*, bool);

	void* alloc(std::size_t count, std::size_t al)
	{
		obj_ptr<MemoryManager, AddressTable::MemoryManager> memoryMngr;
		fcn_ptr<MemoryManager__Allocate_t, AddressTable::MemoryManager__Allocate> MemoryManager__Allocate;

		if (void* ptr = MemoryManager__Allocate(memoryMngr.get(), count, al, al > __STDCPP_DEFAULT_NEW_ALIGNMENT__)) {
			return ptr;
		}
		throw std::bad_alloc();
	}

	void free(void* ptr, std::size_t al)
	{
		obj_ptr<MemoryManager, AddressTable::MemoryManager> memoryMngr;
		fcn_ptr<MemoryManager__Free_t, AddressTable::MemoryManager__Free> MemoryManager__Free;

		MemoryManager__Free(memoryMngr.get(), ptr, al > __STDCPP_DEFAULT_NEW_ALIGNMENT__);
	}
}
