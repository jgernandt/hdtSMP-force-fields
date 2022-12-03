/*
This file is part of CommonLibSSE, with minor changes.

Copyright 2018 Ryan-rsm-McKenzie

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once

template<typename T>
class ni_ptr
{
public:
	using element_type = T;

	// 1
	constexpr ni_ptr() noexcept = default;

	// 2
	constexpr ni_ptr(std::nullptr_t) noexcept {}

	// 3
	template <
		typename Y,
		std::enable_if_t<
		std::is_convertible_v<
		Y*,
		element_type*>,
		int> = 0>
	explicit ni_ptr(Y* rhs) :
		_ptr(rhs)
	{
		TryAttach();
	}

	// 9a
	ni_ptr(const ni_ptr& rhs) :
		_ptr(rhs._ptr)
	{
		TryAttach();
	}

	// 9b
	template <
		typename Y,
		std::enable_if_t<
		std::is_convertible_v<
		Y*,
		element_type*>,
		int> = 0>
	ni_ptr(const ni_ptr<Y>& rhs) :
		_ptr(rhs._ptr)
	{
		TryAttach();
	}

	// 10a
	ni_ptr(ni_ptr&& rhs) noexcept :
		_ptr(rhs._ptr)
	{
		rhs._ptr = nullptr;
	}

	// 10b
	template <
		typename Y,
		std::enable_if_t<
		std::is_convertible_v<
		Y*,
		element_type*>,
		int> = 0>
	ni_ptr(ni_ptr<Y>&& rhs) noexcept :
		_ptr(rhs._ptr)
	{
		rhs._ptr = nullptr;
	}

	~ni_ptr() { TryDetach(); }

	// 1a
	ni_ptr& operator=(const ni_ptr& rhs)
	{
		if (this != std::addressof(rhs)) {
			TryDetach();
			_ptr = rhs._ptr;
			TryAttach();
		}
		return *this;
	}

	// 1b
	template <
		typename Y,
		std::enable_if_t<
		std::is_convertible_v<
		Y*,
		element_type*>,
		int> = 0>
	ni_ptr& operator=(const ni_ptr<Y>& rhs)
	{
		TryDetach();
		_ptr = rhs._ptr;
		TryAttach();
		return *this;
	}

	// 2a
	ni_ptr& operator=(ni_ptr&& rhs)
	{
		if (this != std::addressof(rhs)) {
			TryDetach();
			_ptr = rhs._ptr;
			rhs._ptr = nullptr;
		}
		return *this;
	}

	// 2b
	template <
		typename Y,
		std::enable_if_t<
		std::is_convertible_v<
		Y*,
		element_type*>,
		int> = 0>
	ni_ptr& operator=(ni_ptr<Y>&& rhs)
	{
		TryDetach();
		_ptr = rhs._ptr;
		rhs._ptr = nullptr;
		return *this;
	}

	void reset() { TryDetach(); }

	template <
		typename Y,
		std::enable_if_t<
		std::is_convertible_v<
		Y*,
		element_type*>,
		int> = 0>
	void reset(Y* ptr)
	{
		if (_ptr != ptr) {
			TryDetach();
			_ptr = ptr;
			TryAttach();
		}
	}

	[[nodiscard]] constexpr element_type* get() const noexcept
	{
		return _ptr;
	}

	[[nodiscard]] explicit constexpr operator bool() const noexcept
	{
		return static_cast<bool>(_ptr);
	}

	[[nodiscard]] constexpr element_type& operator*() const noexcept
	{
		assert(static_cast<bool>(*this));
		return *_ptr;
	}

	[[nodiscard]] constexpr element_type* operator->() const noexcept
	{
		assert(static_cast<bool>(*this));
		return _ptr;
	}

protected:
	template <class>
	friend class ni_ptr;

	void TryAttach()
	{
		if (_ptr) {
			_ptr->IncRefCount();
		}
	}

	void TryDetach()
	{
		if (_ptr) {
			_ptr->DecRefCount();
			_ptr = nullptr;
		}
	}

	// members
	element_type* _ptr{ nullptr };  // 0
};
//static_assert(sizeof(ni_ptr<void*>) == 0x8);

template<typename T, class... Args>
[[nodiscard]] ni_ptr<T> make_nismart(Args&&... args)
{
	return ni_ptr<T>{ new T(std::forward<Args>(args)...) };
}

template<typename T1, typename T2>
[[nodiscard]] constexpr bool operator==(const ni_ptr<T1>& lhs, const ni_ptr<T2>& rhs)
{
	return lhs.get() == rhs.get();
}

template<typename T1, typename T2>
[[nodiscard]] constexpr bool operator!=(const ni_ptr<T1>& lhs, const ni_ptr<T2>& rhs)
{
	return !(lhs == rhs);
}

template<typename T>
[[nodiscard]] constexpr bool operator==(const ni_ptr<T>& lhs, std::nullptr_t) noexcept
{
	return !lhs;
}

template<typename T>
[[nodiscard]] constexpr bool operator==(std::nullptr_t, const ni_ptr<T>& rhs) noexcept
{
	return !rhs;
}

template<typename T>
[[nodiscard]] constexpr bool operator!=(const ni_ptr<T>& lhs, std::nullptr_t) noexcept
{
	return static_cast<bool>(lhs);
}

template<typename T>
[[nodiscard]] constexpr bool operator!=(std::nullptr_t, const ni_ptr<T>& rhs) noexcept
{
	return static_cast<bool>(rhs);
}

template<typename T>
ni_ptr(T*)->ni_ptr<T>;
