#pragma once
#include "obj_traits.h"
#include "ModelReferenceEffect.h"
#include "utils.h"

namespace jg
{
	template<>
	struct obj_traits<Skyrim::ModelReferenceEffect>
	{
		constexpr static bool hasAge() { return true; }
		static float age(const Skyrim::ModelReferenceEffect& obj) { return obj.getAge(); }

		constexpr static float duration(const Skyrim::ModelReferenceEffect& obj) { return -1.0f; }
		static bool expired(const Skyrim::ModelReferenceEffect& obj) { return obj.getFinished(); }

		constexpr static float force(const Skyrim::ModelReferenceEffect& obj) { return 0.0f; }

		constexpr static float length(const Skyrim::ModelReferenceEffect& obj) { return 0.0f; }
		constexpr static float radius(const Skyrim::ModelReferenceEffect& obj) { return 0.0f; }

		static btVector3 translate(const Skyrim::ModelReferenceEffect& obj, const btVector3& t)
		{
			return toWorld(obj.get3D(), t);
		}
		static btMatrix3x3 rotate(const Skyrim::ModelReferenceEffect& obj, const btMatrix3x3& r)
		{
			return toWorld(obj.get3D(), r);
		}
		static float scale(const Skyrim::ModelReferenceEffect& obj, float s)
		{
			return toWorld(obj.get3D(), s);
		}

		static bool isAttached(const Skyrim::ModelReferenceEffect& obj);
	};
}
