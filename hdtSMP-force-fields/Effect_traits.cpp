#include "pch.h"
#include "Effect_traits.h"
#include "ObjRef_traits.h"

bool jg::obj_traits<Skyrim::ModelReferenceEffect>::isAttached(const Skyrim::ModelReferenceEffect& obj)
{
	if (auto target = obj.getTarget()) {
		return obj_traits<Skyrim::ObjectReference>::isAttached(*target);
	}
	else {
		return false;
	}
}
