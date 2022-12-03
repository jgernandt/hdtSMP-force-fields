#pragma once
#include <cstddef>

namespace Skyrim
{
	void* alloc(std::size_t count, std::size_t al = __STDCPP_DEFAULT_NEW_ALIGNMENT__);
	void free(void* ptr, std::size_t al = __STDCPP_DEFAULT_NEW_ALIGNMENT__);

	template<typename T>
	struct allocator
	{
		using value_type = T;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		constexpr allocator() noexcept = default;
		constexpr allocator(const allocator& other) noexcept = default;
		template<typename U> constexpr allocator(const allocator<U>& other) noexcept {}

		constexpr ~allocator() = default;

		[[nodiscard]] constexpr T* allocate(std::size_t n)
		{
			static_assert(sizeof(T) % alignof(T) == 0);//this is guaranteed in general, right?
			return reinterpret_cast<T*>(alloc(n * sizeof(T), alignof(T)));
		}

		constexpr void deallocate(T* p, std::size_t)
		{
			free(p, alignof(T));
		}
	};

	template<typename T, typename U>
	constexpr bool operator==(const allocator<T>& lhs, const allocator<U>& rhs) noexcept
	{
		return true;
	}
}
