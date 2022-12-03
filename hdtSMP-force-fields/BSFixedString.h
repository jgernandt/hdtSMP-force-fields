#pragma once

namespace Skyrim
{
	class BSFixedString
	{
	public:
		BSFixedString() = default;
		explicit BSFixedString(const char* str);
		BSFixedString(const BSFixedString& other);
		BSFixedString(BSFixedString&& other) { swap(other); }

		~BSFixedString();

		BSFixedString& operator=(const char* str);
		BSFixedString& operator=(const BSFixedString& other);
		BSFixedString& operator=(BSFixedString&& other)
		{
			if (this != &other) {
				BSFixedString tmp;
				swap(tmp);
				swap(other);
			}
			return *this;
		}

		void swap(BSFixedString& other) { std::swap(m_string, other.m_string); }

		const char* c_str() const { return m_string; }

	private:
		const char* m_string{ nullptr };

		friend bool operator==(const BSFixedString& lhs, const BSFixedString& rhs) = default;
		friend auto operator<=>(const BSFixedString& lhs, const BSFixedString& rhs) = default;
	};
	static_assert(sizeof(BSFixedString) == sizeof(const char*));
}
