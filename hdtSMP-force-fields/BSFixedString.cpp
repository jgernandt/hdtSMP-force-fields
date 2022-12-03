#include "pch.h"
#include "BSFixedString.h"
#include "Relocation.h"

using BSFixedString__ctor_t = Skyrim::BSFixedString* (*)(Skyrim::BSFixedString*, const char*);
using BSFixedString__ctor_copy_t = Skyrim::BSFixedString * (*)(Skyrim::BSFixedString*, const Skyrim::BSFixedString&);
using BSFixedString__assign_t = Skyrim::BSFixedString * (*)(Skyrim::BSFixedString*, const char*);
using BSFixedString__assign_copy_t = Skyrim::BSFixedString * (*)(Skyrim::BSFixedString*, const Skyrim::BSFixedString&);
using BSFixedString__Release_t = void (*)(Skyrim::BSFixedString*);

Skyrim::BSFixedString::BSFixedString(const char* str)
{
	fcn_ptr<BSFixedString__ctor_t, AddressTable::BSFixedString__ctor>{}(this, str);
}

Skyrim::BSFixedString::BSFixedString(const BSFixedString& other)
{
	fcn_ptr<BSFixedString__ctor_copy_t, AddressTable::BSFixedString__ctor_copy>{}(this, other);
}

Skyrim::BSFixedString::~BSFixedString()
{
	fcn_ptr<BSFixedString__Release_t, AddressTable::BSFixedString__Release>{}(this);
}

Skyrim::BSFixedString& Skyrim::BSFixedString::operator=(const char* str) 
{ 
	fcn_ptr<BSFixedString__assign_t, AddressTable::BSFixedString__assign>{}(this, str); return *this;
}

Skyrim::BSFixedString& Skyrim::BSFixedString::operator=(const BSFixedString& other)
{
	fcn_ptr<BSFixedString__assign_copy_t, AddressTable::BSFixedString__assign_copy>{}(this, other); return *this;
}
