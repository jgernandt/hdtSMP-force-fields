#include "pch.h"
#include "ForceFieldManager.h"
#include "Objects.h"
#include "relocation.h"

using namespace jg;

using ObjectReference__Load3D_t = Skyrim::NiAVObject * (*)(Skyrim::ObjectReference*, bool);
using ObjectReference__Release3D_t = void (*)(Skyrim::ObjectReference*);

using ModelReferenceEffect__DetachImpl_t = void(*)(Skyrim::ModelReferenceEffect*);
using ModelReferenceEffect__Unk57bcc0_t = std::uint64_t(*)(Skyrim::ModelReferenceEffect*);

namespace target
{
	ObjectReference__Load3D_t ObjectReference__Load3D;
	ObjectReference__Release3D_t ObjectReference__Release3D;

	ModelReferenceEffect__DetachImpl_t ModelReferenceEffect__DetachImpl;
	ModelReferenceEffect__Unk57bcc0_t ModelReferenceEffect__Unk57bcc0;
}

namespace detour
{
	Skyrim::NiAVObject* ObjectReference__Load3D(Skyrim::ObjectReference* _this, bool bkgnd)
	{
		auto result = target::ObjectReference__Load3D(_this, bkgnd);
		if (result) {
			jg::ForceFieldManager::get().onAttach(result, _this);
		}
		return result;
	}

	void ObjectReference__Release3D(Skyrim::ObjectReference* _this)
	{
		assert(_this);

		if (auto model = _this->get3D()) {
			jg::ForceFieldManager::get().onDetach(model, _this);
		}
		
		target::ObjectReference__Release3D(_this);
	}

	void ModelReferenceEffect__DetachImpl(Skyrim::ModelReferenceEffect* _this)
	{
		assert(_this);

		if (auto model = _this->get3D()) {
			ForceFieldManager::get().onDetach(model, _this);
		}
		target::ModelReferenceEffect__DetachImpl(_this);
	}

	//This is part of the update procedure and will run every frame.
	//Ideally, we should move the hook to +0x86, which is where the attaching begins.
	//That may be less portable, though.
	std::uint64_t ModelReferenceEffect__Unk57bcc0(Skyrim::ModelReferenceEffect* _this)
	{
		assert(_this);

		Skyrim::NiAVObject* old_model = _this->get3D();

		std::uint64_t result = target::ModelReferenceEffect__Unk57bcc0(_this);

		if (!old_model) {
			if (auto model = _this->get3D()) {
				ForceFieldManager::get().onAttach(model, _this);
			}
		}

		return result;
	}
}

template<std::size_t ID, typename T> requires std::is_pointer_v<T>
bool hookFcn(T& target, T detour, const char* name)
{
	target = fcn_ptr<T, ID>().get();
	if (LONG res = DetourAttach(&target, detour) != NO_ERROR) {
		_ERROR("ERROR: Detouring %s failed with error code %d", name, res);
		return false;
	}
	_VMESSAGE("Attaching detour to %s...", name);
	return true;
}

#define HOOK_FCN(name) hookFcn<AddressTable::name>(target::name, &detour::name, #name)

bool attachHooks()
{
	if (LONG res = DetourTransactionBegin() != NO_ERROR) {
		_ERROR("ERROR: DetourTransactionBegin failed with error code %d", res);
		return false;
	}

	if (!HOOK_FCN(ModelReferenceEffect__DetachImpl)) return false;
	if (!HOOK_FCN(ModelReferenceEffect__Unk57bcc0)) return false;
	if (!HOOK_FCN(ObjectReference__Load3D)) return false;
	if (!HOOK_FCN(ObjectReference__Release3D)) return false;

	if (LONG res = DetourTransactionCommit() != NO_ERROR) {
		_ERROR("ERROR: DetourTransactionCommit failed with error code %d", res);
		return false;
	}

	return true;
}
