#pragma once
#include <cstddef>
#include <list>
#include <vector>

class SizeManager
{
private:
	static SizeManager& get()
	{
		static SizeManager instance;
		return instance;
	}

public:
	static void init(unsigned int version);

private:
	SizeManager() = default;
	~SizeManager() = default;

private:
	friend class var_offset;

	void queryOffset(const var_offset& base, std::size_t offset, var_offset* caller, const char* desc);
	void querySize(const std::initializer_list<std::size_t>& sizes, var_offset* caller, const char* desc);

private:
	struct SizeData
	{
		std::size_t sizes[2];
		var_offset* caller;
		const char* desc;
	};
	std::vector<SizeData> m_sizes;

	struct OffsetData
	{
		const var_offset* base;
		std::size_t offset;
		var_offset* caller;
		const char* desc;
	};
	std::list<OffsetData> m_offsets;
};

class var_offset
{
public:
	var_offset(const std::initializer_list<std::size_t>& sizes, const char* desc)
	{
		SizeManager::get().querySize(sizes, this, desc);
	}
	//Note: constructing from an initializer list of { var_offset, size_t }
	// will prefer the first ctor (var_offset implicitly converts to size_t)
	var_offset(const var_offset& base, std::size_t offset, const char* desc)
	{
		SizeManager::get().queryOffset(base, offset, this, desc);
	}

	var_offset(const var_offset&) = delete;
	var_offset& operator=(const var_offset&) = delete;

	~var_offset() = default;

	operator std::size_t() const { return m_value; }

private:
	std::size_t m_value{ 0 };

	friend class SizeManager;
};
