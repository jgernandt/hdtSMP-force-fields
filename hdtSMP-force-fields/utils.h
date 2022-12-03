#pragma once
#include "BulletAPI.h"

constexpr float PI = 3.1415927f;

namespace Skyrim
{
	class NiAVObject;
}

namespace jg
{
	float randf(float min = 0.0f, float max = 1.0f);

	btVector3 toWorld(Skyrim::NiAVObject* parent, const btVector3& translation);
	btMatrix3x3 toWorld(Skyrim::NiAVObject* parent, const btMatrix3x3& rotation);
	float toWorld(Skyrim::NiAVObject* parent, float scale);
}
