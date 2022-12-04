#pragma once
#include <cstddef>
#include <cstdint>
#include <type_traits>

//Looking for a modular approach to this, but for now this is just stupidly complicated...

union Address
{
	std::uint32_t ids[2];
	std::uint64_t address;
};

template<typename... Ts>
struct address_table
{};

template<>
struct address_table<>
{
	constexpr static std::size_t COUNT = 0;

	Address& operator[](std::size_t i) const { return *((Address*)this + i); }
};

namespace Skyrim { class BSFixedString; }
template<typename... Ts>
struct address_table<Skyrim::BSFixedString, Ts...> : address_table<Ts...>
{
	constexpr static std::size_t BSFixedString__ctor{ address_table<Ts...>::COUNT };
	Address _BSFixedString__ctor{ 67819, 69161 };

	constexpr static std::size_t BSFixedString__ctor_copy{ address_table<Ts...>::COUNT + 1 };
	Address _BSFixedString__ctor_copy{ 67820, 69162 };

	constexpr static std::size_t BSFixedString__assign{ address_table<Ts...>::COUNT + 2 };
	Address _BSFixedString__assign{ 67823, 69165 };

	constexpr static std::size_t BSFixedString__assign_copy{ address_table<Ts...>::COUNT + 3 };
	Address _BSFixedString__assign_copy{ 67824, 69166 };

	constexpr static std::size_t BSFixedString__Release{ address_table<Ts...>::COUNT + 4 };
	Address _BSFixedString__Release{ 67847, 69192 };

	constexpr static std::size_t COUNT{ address_table<Ts...>::COUNT + 5 };
};

namespace Skyrim { class MemoryManager; }
template<typename... Ts>
struct address_table<Skyrim::MemoryManager, Ts...> : address_table<Ts...>
{
	constexpr static std::size_t MemoryManager__Allocate{ address_table<Ts...>::COUNT };
	Address _MemoryManager__Allocate{ 66859, 68115 };

	constexpr static std::size_t MemoryManager__Free{ address_table<Ts...>::COUNT + 1 };
	Address _MemoryManager__Free{ 66861, 68117 };

	constexpr static std::size_t MemoryManager{ address_table<Ts...>::COUNT + 2 };
	Address _MemoryManager{ 514110, 400188 };

	constexpr static std::size_t COUNT{ address_table<Ts...>::COUNT + 3 };
};

namespace Skyrim { class ObjectReference; }
template<typename... Ts>
struct address_table<Skyrim::ObjectReference, Ts...> : address_table<Ts...>
{
	constexpr static std::size_t LookupRefByHandle{ address_table<Ts...>::COUNT };
	Address _LookupRefByHandle{ 12204, 12332 };

	constexpr static std::size_t ObjectReference__Load3D{ address_table<Ts...>::COUNT + 1 };
	Address _ObjectReference__Load3D{ 19300, 19727 };

	constexpr static std::size_t ObjectReference__Release3D{ address_table<Ts...>::COUNT + 2 };
	Address _ObjectReference__Release3D{ 19301, 19728 };

	constexpr static std::size_t COUNT{ address_table<Ts...>::COUNT + 3 };
};

namespace Skyrim { class ModelReferenceEffect; }
template<typename... Ts>
struct address_table<Skyrim::ModelReferenceEffect, Ts...> : address_table<Ts...>
{
	constexpr static std::size_t ModelReferenceEffect__DetachImpl{ address_table<Ts...>::COUNT };
	Address _ModelReferenceEffect__DetachImpl{ 33869, 34665 };

	constexpr static std::size_t ModelReferenceEffect__Unk57bcc0{ address_table<Ts...>::COUNT + 1 };
	Address _ModelReferenceEffect__Unk57bcc0{ 33873, 34669 };

	constexpr static std::size_t COUNT{ address_table<Ts...>::COUNT + 2 };
};

namespace Skyrim { class NiStringsExtraData; }
template<typename... Ts>
struct address_table<Skyrim::NiStringsExtraData, Ts...> : address_table<Ts...>
{
	constexpr static std::size_t NiRTTI_NiStringsExtraData{ address_table<Ts...>::COUNT };
	Address _NiRTTI_NiStringsExtraData{ 523925, 410505 };

	constexpr static std::size_t COUNT{ address_table<Ts...>::COUNT + 1 };
};

using AddressTable = address_table<
	Skyrim::BSFixedString,
	Skyrim::MemoryManager,
	Skyrim::ModelReferenceEffect,
	Skyrim::NiStringsExtraData,
	Skyrim::ObjectReference>;

extern AddressTable g_addressTable;

struct SkyrimVersion;
bool initAddressTable(const SkyrimVersion& version);

template<std::size_t ID>
class reloc_addr
{
protected:
	reloc_addr() = default;
	~reloc_addr() = default;

	std::uintptr_t address() const { return g_addressTable[ID].address; }
};

template<typename T, std::size_t ID>
class fcn_ptr final : public reloc_addr<ID>
{
public:
	fcn_ptr() = default;
	~fcn_ptr() = default;

	T get() { assert(this->address()); return reinterpret_cast<T>(this->address()); }

	template<typename... Args> requires std::is_invocable_v<T, Args...>
	std::invoke_result_t<T, Args...> operator() (Args&&... args) 
	{
		assert(this->address());
		return std::invoke(std::forward<T>(reinterpret_cast<T>(this->address())), std::forward<Args>(args)...);
	}
};

template<typename T, std::size_t ID>
class obj_ptr final : public reloc_addr<ID>
{
public:
	obj_ptr() = default;
	~obj_ptr() = default;

	T* get() { assert(this->address()); return reinterpret_cast<T*>(this->address()); }
	const T* get() const { assert(this->address()); return reinterpret_cast<T*>(this->address()); }

	T* operator->() { assert(this->address()); return reinterpret_cast<T*>(this->address()); }
	T& operator*() { assert(this->address()); return *reinterpret_cast<T*>(this->address()); }

	friend bool operator==(const obj_ptr<T, ID>& lhs, const T* rhs) { return lhs.get() == rhs; }
	friend bool operator==(const T* lhs, const obj_ptr<T, ID>& rhs) { return rhs == lhs; }
	friend bool operator!=(const obj_ptr<T, ID>& lhs, const T* rhs) { return !(lhs == rhs); }
	friend bool operator!=(const T* lhs, const obj_ptr<T, ID>& rhs) { return !(rhs == lhs); }
};

template<std::size_t ID>
class vtbl_ptr final : public reloc_addr<ID>
{
public:
	vtbl_ptr() = default;
	~vtbl_ptr() = default;

	void** get(std::size_t i = 0) { assert(this->address()); return reinterpret_cast<void**>(this->address() + i * sizeof(void*)); }

	void* operator[] (std::size_t i) const { assert(this->address()); return *reinterpret_cast<void**>(this->address() + i * sizeof(void*)); }
};
