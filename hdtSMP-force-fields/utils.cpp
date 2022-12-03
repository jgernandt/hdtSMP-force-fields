#include "pch.h"
#include "NiAVObject.h"
#include "Relocation.h"
#include "utils.h"

thread_local std::minstd_rand g_rng;

float jg::randf(float min, float max)
{
	assert(max > min);
	std::uniform_real_distribution<float> d(min, max);
	return d(g_rng);
}

btVector3 jg::toWorld(Skyrim::NiAVObject* parent, const btVector3& translation)
{
	if (parent) {
		auto&& t = parent->getWorldTransform().t;
		auto&& A = parent->getWorldTransform().r;

		//result = parent position + parent scale * parent rotation * translation

		btVector3 result(
			A(0, 0) * translation[0] + A(0, 1) * translation[1] + A(0, 2) * translation[2],
			A(1, 0) * translation[0] + A(1, 1) * translation[1] + A(1, 2) * translation[2],
			A(2, 0) * translation[0] + A(2, 1) * translation[1] + A(2, 2) * translation[2]);

		result *= parent->getWorldTransform().s;
		result += btVector3(t.x, t.y, t.z);

		return result;
	}
	else {
		return translation;
	}
}

btMatrix3x3 jg::toWorld(Skyrim::NiAVObject* parent, const btMatrix3x3& rotation)
{
	if (parent) {
		auto&& A = parent->getWorldTransform().r;

		return btMatrix3x3(
			A(0, 0), A(0, 1), A(0, 2),
			A(1, 0), A(1, 1), A(1, 2),
			A(2, 0), A(2, 1), A(2, 2)) * rotation;
	}
	else {
		return rotation;
	}
}

float jg::toWorld(Skyrim::NiAVObject* parent, float scale)
{
	if (parent) {
		return parent->getWorldTransform().s * scale;
	}
	else {
		return scale;
	}
}
