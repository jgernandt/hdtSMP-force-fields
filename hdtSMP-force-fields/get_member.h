#pragma once
#include <type_traits>

template<typename T> requires std::is_fundamental_v<T> || std::is_pointer_v<T>
T get_member(const void* base, uintptr_t offset)
{
	{ return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(base) + offset); }
}

template<typename T> requires std::is_class_v<T>
const T& get_member(const void* base, uintptr_t offset)
{
	{ return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(base) + offset); }
}
