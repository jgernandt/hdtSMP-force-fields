#include "pch.h"
#include "var_size.h"

void SizeManager::init(unsigned int version)
{
	int i = version >= RUNTIME_VERSION_1_6_629 ? 1 : 0;

	for (auto&& d : get().m_sizes) {
		d.caller->m_value = d.sizes[i];
		_VMESSAGE("%s = 0x%x", d.desc ? d.desc : "", d.caller->m_value);
	}

	for (auto&& d : get().m_offsets) {
		d.caller->m_value = d.base->m_value + d.offset;
		_VMESSAGE("%s = 0x%x", d.desc ? d.desc : "", d.caller->m_value);
	}

	get().m_sizes.clear();
	get().m_offsets.clear();
}

void SizeManager::queryOffset(const var_offset& base, std::size_t offset, var_offset* caller, const char* desc)
{
	//If caller is the base of someone else, it needs to be inserted before it in the list.
	//We do not check for circular dependencies.
	auto it = std::find_if(m_offsets.begin(), m_offsets.end(), [caller](const OffsetData& d) { return d.base == caller; });
	m_offsets.insert(it, { &base, offset, caller, desc });
}

void SizeManager::querySize(const std::initializer_list<std::size_t>& sizes, var_offset* caller, const char* desc)
{
	assert(sizes.size() <= 3);

	SizeData d;
	int i = 0;
	for (auto it = sizes.begin(); it != sizes.end(); ++it) {
		d.sizes[i++] = *it;
	}
	d.caller = caller;
	d.desc = desc;

	m_sizes.push_back(d);
}
