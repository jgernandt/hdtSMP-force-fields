#pragma once
#include <cassert>
#include <filesystem>

namespace jg
{
	class Config
	{
	public:
		enum BoolID : int
		{
			LOG_STATS,

			BOOL_COUNT
		};
		enum FloatID : int
		{
			FORCE_MULTIPLIER,

			FLOAT_COUNT
		};
		enum IntID : int
		{
			LOG_LEVEL,
			THREADS,

			INT_COUNT
		};

	public:
		Config();

		bool load(const std::filesystem::path& path);

		bool getb(int id) const { assert(id >= 0 && id < BOOL_COUNT); return m_bools[id]; }
		void set(int id, bool b);

		float getf(int id) const { assert(id >= 0 && id < FLOAT_COUNT); return m_floats[id]; }
		void set(int id, float f);

		int geti(int id) const { assert(id >= 0 && id < INT_COUNT); return m_ints[id]; }
		void set(int id, int i);

	private:
		std::filesystem::path m_path;
		float m_floats[FLOAT_COUNT ? FLOAT_COUNT : 1];
		int m_ints[INT_COUNT ? INT_COUNT : 1];
		bool m_bools[BOOL_COUNT ? BOOL_COUNT : 1];
	};

	extern Config g_config;
}
