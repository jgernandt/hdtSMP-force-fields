#pragma once
#include <cassert>

namespace Skyrim
{
	struct NiPoint3
	{
		float x;
		float y;
		float z;

		float& operator[](int i) { assert(i >= 0 && i < 3); return (&x)[i]; }
		const float& operator[](int i) const { assert(i >= 0 && i < 3); return (&x)[i]; }
	};
	static_assert(sizeof(NiPoint3) == 0x0c);

	struct NiMatrix33
	{
		union
		{
			float	rc[3][3];
			float   arr[9];
		};

		float& operator()(int r, int c) { return rc[r][c]; }
		const float& operator()(int r, int c) const { return rc[r][c]; }
	};
	static_assert(sizeof(NiMatrix33) == 0x24);

	struct NiTransform
	{
		NiMatrix33 r;
		NiPoint3 t;
		float s;
	};
	static_assert(sizeof(NiTransform) == 0x34);

	struct NiBound
	{
		NiPoint3 centre;
		float radius;
	};
	static_assert(sizeof(NiBound) == 0x10);
}
