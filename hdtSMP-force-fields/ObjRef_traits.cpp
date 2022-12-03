#include "pch.h"
#include "Cell.h"
#include "ObjRef_traits.h"

bool jg::obj_traits<Skyrim::ObjectReference>::isAttached(const Skyrim::ObjectReference& obj)
{
	return obj.getParentCell() && obj.getParentCell()->isAttached();
}
