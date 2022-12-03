#pragma once
#include "obj_traits.h"
#include "ObjectReference.h"
#include "utils.h"

namespace jg
{
	template<>
	struct obj_traits<Skyrim::ObjectReference>
	{
		constexpr static bool hasAge() { return false; }
		constexpr static float age(const Skyrim::ObjectReference& obj) { return 0.0f; }

		constexpr static float duration(const Skyrim::ObjectReference& obj) { return -1.0f; }
		constexpr static bool expired(const Skyrim::ObjectReference& obj) { return false; }

		constexpr static float force(const Skyrim::ObjectReference& obj) { return 0.0f; }

		constexpr static float length(const Skyrim::ObjectReference& obj) { return 0.0f; }
		constexpr static float radius(const Skyrim::ObjectReference& obj) { return 0.0f; }

		static btVector3 translate(const Skyrim::ObjectReference& obj, const btVector3& t)
		{
			return toWorld(obj.get3D(), t);
		}
		static btMatrix3x3 rotate(const Skyrim::ObjectReference& obj, const btMatrix3x3& r)
		{
			return toWorld(obj.get3D(), r);
		}
		static float scale(const Skyrim::ObjectReference& obj, float s)
		{
			return toWorld(obj.get3D(), s);
		}

		static bool isAttached(const Skyrim::ObjectReference& obj);
	};
}
