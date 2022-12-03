#include "pch.h"
#include "ModelReferenceEffect.h"
#include "ObjectReference.h"
#include "Relocation.h"

ni_ptr<Skyrim::ObjectReference> Skyrim::ReferenceEffect::getTarget() const
{
	fcn_ptr<LookupRefByHandle_t, AddressTable::LookupRefByHandle> LookupRefByHandle;

	ni_ptr<ObjectReference> result;
	LookupRefByHandle(get_member<std::uint32_t>(this, offsetof_target), result);
	return result;
}

