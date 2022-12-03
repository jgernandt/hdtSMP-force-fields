#include "pch.h"
#include "Config.h"

jg::Config jg::g_config;

template<typename T>
struct Option
{
	const char* header;
	const char* key;
	T def;
};

constexpr Option<bool> BOOLS[jg::Config::BOOL_COUNT ? jg::Config::BOOL_COUNT : 1]{
	{ "General", "bLogStatistics", false },
};

constexpr Option<float> FLOATS[jg::Config::FLOAT_COUNT ? jg::Config::FLOAT_COUNT : 1]{
	{ "ForceFields", "fForceMultiplier", 1.0f },
};

constexpr Option<int> INTS[jg::Config::INT_COUNT ? jg::Config::INT_COUNT : 1]{
	{ "General", "iLogLevel", 4 },
	{ "Performance", "iThreads", 8 },
};

jg::Config::Config()
{
	for (int i = 0; i < BOOL_COUNT; i++) {
		m_bools[i] = BOOLS[i].def;
	}
	for (int i = 0; i < FLOAT_COUNT; i++) {
		m_floats[i] = FLOATS[i].def;
	}
	for (int i = 0; i < INT_COUNT; i++) {
		m_ints[i] = INTS[i].def;
	}
}

bool jg::Config::load(const std::filesystem::path& path)
{
	if (path.extension().string() != ".ini") {
		return false;
	}
	else {
		m_path = path;

		if (std::filesystem::exists(path)) {
			char buf[256];
			for (int i = 0; i < BOOL_COUNT; i++) {
				DWORD res = GetPrivateProfileString(
					BOOLS[i].header, BOOLS[i].key, NULL, buf, sizeof(buf), m_path.string().c_str());
				if (res == 0) {
					m_bools[i] = BOOLS[i].def;
				}
				else {
					errno = 0;
					char* end;
					int result = std::strtol(buf, &end, 10);
					if (end == &buf[0] || errno == ERANGE) {
						_WARNING("WARNING: invalid boolean value %s for %s/%s. Using default.",
							buf, BOOLS[i].header, BOOLS[i].key);
						m_bools[i] = BOOLS[i].def;
					}
					else {
						m_bools[i] = static_cast<bool>(result);
					}
				}
			}

			for (int i = 0; i < FLOAT_COUNT; i++) {
				DWORD res = GetPrivateProfileString(
					FLOATS[i].header, FLOATS[i].key, NULL, buf, sizeof(buf), m_path.string().c_str());
				if (res == 0) {
					m_floats[i] = FLOATS[i].def;
				}
				else {
					errno = 0;
					char* end;
					float result = std::strtof(buf, &end);
					if (end == &buf[0] || errno == ERANGE) {
						_WARNING("WARNING: invalid float value %s for %s/%s. Using default.", 
							buf, FLOATS[i].header, FLOATS[i].key);
						m_floats[i] = FLOATS[i].def;
					}
					else {
						m_floats[i] = result;
					}
				}
			}
			for (int i = 0; i < INT_COUNT; i++) {
				DWORD res = GetPrivateProfileString(
					INTS[i].header, INTS[i].key, NULL, buf, sizeof(buf), m_path.string().c_str());
				if (res == 0) {
					m_ints[i] = INTS[i].def;
				}
				else {
					errno = 0;
					char* end;
					int result = std::strtol(buf, &end, 10);
					if (end == &buf[0] || errno == ERANGE) {
						_WARNING("WARNING: invalid integer value %s for %s/%s. Using default.", 
							buf, INTS[i].header, INTS[i].key);
						m_ints[i] = INTS[i].def;
					}
					else {
						m_ints[i] = result;
					}
				}
			}
		}
		//If file does not exist, create one with default settings
		else {
			for (int i = 0; i < BOOL_COUNT; i++) {
				set(i, BOOLS[i].def);
			}
			for (int i = 0; i < FLOAT_COUNT; i++) {
				set(i, FLOATS[i].def);
			}
			for (int i = 0; i < INT_COUNT; i++) {
				set(i, INTS[i].def);
			}
		}
		return true;
	}
}

void jg::Config::set(int id, bool b)
{
	assert(id >= 0 && id < BOOL_COUNT);

	m_bools[id] = b;

	WritePrivateProfileString(BOOLS[id].header, BOOLS[id].key, b ? "1" : "0", m_path.string().c_str());
}

void jg::Config::set(int id, float f)
{
	assert(id >= 0 && id < FLOAT_COUNT);

	m_floats[id] = f;

	char buf[16];
	snprintf(buf, sizeof(buf), "%f", m_floats[id]);
	WritePrivateProfileString(FLOATS[id].header, FLOATS[id].key, buf, m_path.string().c_str());
}

void jg::Config::set(int id, int i)
{
	assert(id >= 0 && id < INT_COUNT);

	m_ints[id] = i;

	char buf[16];
	snprintf(buf, sizeof(buf), "%d", m_ints[id]);
	auto res = WritePrivateProfileString(INTS[id].header, INTS[id].key, buf, m_path.string().c_str());
}
