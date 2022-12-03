#pragma once
#include <memory>
#include <string>
#include <vector>

#include "BSFixedString.h"
#include "memory.h"

class btRigidBody;

namespace Skyrim
{
	class ModelReferenceEffect;
	class ObjectReference;
}

namespace jg
{
	class IForceField
	{
	public:
		virtual ~IForceField() = default;
		virtual void actOn(btRigidBody& body) const = 0;
		//update internal state, return true if active
		virtual bool update(float dt) = 0;
		
		void* operator new(std::size_t count) { return Skyrim::alloc(count); }
		void* operator new(std::size_t count, std::align_val_t al) { return Skyrim::alloc(count, (std::size_t)al); }

		void operator delete(void* ptr) { return Skyrim::free(ptr); }
		void operator delete(void* ptr, std::align_val_t al) { return Skyrim::free(ptr, (std::size_t)al); }
		
	};

	union ParamVal
	{
		bool b;
		float f;
		int i;
		float v[4];
	};

	using ParamMap = std::map<Skyrim::BSFixedString, ParamVal, std::less<Skyrim::BSFixedString>, Skyrim::allocator<std::pair<const Skyrim::BSFixedString, ParamVal>>>;

	class IForceFieldFactory
	{
	public:
		virtual ~IForceFieldFactory() = default;

		[[nodiscard]] virtual std::unique_ptr<IForceField> create(Skyrim::ModelReferenceEffect* effect, const ParamMap& params) = 0;
		[[nodiscard]] virtual std::unique_ptr<IForceField> create(Skyrim::ObjectReference* obj, const ParamMap& params) = 0;
	};
}
