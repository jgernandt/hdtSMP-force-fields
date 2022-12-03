#pragma once
#include "BulletAPI.h"

namespace jg
{
	template<typename T>
	struct obj_traits
	{
		constexpr static bool hasAge() { return false; }
		constexpr static float age(const T& obj) { return 0.0f; }

		constexpr static float duration(const T& obj) { return -1.0f; }
		constexpr static bool expired(const T& obj) { return false; }

		constexpr static float force(const T& obj) { return 0.0f; }

		constexpr static float length(const T& obj) { return 0.0f; }
		constexpr static float radius(const T& obj) { return 0.0f; }

		constexpr static const btVector3& translate(const T& obj, const btVector3& t) { return t; }
		constexpr static const btMatrix3x3& rotate(const T& obj, const btMatrix3x3& r) { return r; }
		constexpr static float scale(const T& obj, float s) { return s; }

		constexpr static bool isAttached(const T& obj) { return false; }
	};
}
