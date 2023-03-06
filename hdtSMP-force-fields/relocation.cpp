#include "pch.h"
#include "Relocation.h"
#include "version.h"

#define VersionDb AddressLib_1_5
#include "versiondb.h"
#undef VersionDb

#define VersionDb AddressLib_1_6
#include "versionlibdb.h"
#undef VersionDb

class IAddressLib
{
public:
	~IAddressLib();

	bool load(const SkyrimVersion& version);
	void* findAddressById(unsigned long long id) const;
	bool findIdByOffset(unsigned long long offset, unsigned long long& result) const;
	bool findOffsetById(unsigned long long id, unsigned long long& result) const;

private:
	void unload();

	enum Version
	{
		NONE,
		V1_5,
		V1_6,
	};
	Version loadedVersion{ NONE };
	void* loadedDb{ nullptr };
};

AddressTable g_addressTable{};

bool initAddressTable(const SkyrimVersion& version)
{
	IAddressLib db;
	if (!db.load(version)) {
		_ERROR("ERROR: Failed to load address library version %d.%d.%d.%d", 
			version.major, version.minor, version.revision, 0);
		return false;
	}

	int index = version.full < MAKE_EXE_VERSION(1, 6, 0) ? 0 : 1;

	const char* names[AddressTable::COUNT]{};
	names[AddressTable::LookupRefByHandle] = "LookupRefByHandle";
	names[AddressTable::ObjectReference__Load3D] = "ObjectReference__Load3D";
	names[AddressTable::ObjectReference__Release3D] = "ObjectReference__Release3D";
	names[AddressTable::ModelReferenceEffect__DetachImpl] = "ModelReferenceEffect__DetachImpl";
	names[AddressTable::ModelReferenceEffect__Unk57bcc0] = "ModelReferenceEffect__Unk57bcc0";
	names[AddressTable::MemoryManager__Allocate] = "MemoryManager__Allocate";
	names[AddressTable::MemoryManager__Free] = "MemoryManager__Free";
	names[AddressTable::MemoryManager__Get] = "MemoryManager__Get";
	names[AddressTable::BSFixedString__ctor] = "BSFixedString__ctor";
	names[AddressTable::BSFixedString__ctor_copy] = "BSFixedString__ctor_copy";
	names[AddressTable::BSFixedString__assign] = "BSFixedString__assign";
	names[AddressTable::BSFixedString__assign_copy] = "BSFixedString__assign_copy";
	names[AddressTable::BSFixedString__Release] = "BSFixedString__Release";
	names[AddressTable::NiRTTI_NiStringsExtraData] = "NiRTTI_NiStringsExtraData";

	bool success = true;
	auto base = (std::uintptr_t)GetModuleHandle(NULL);
	for (std::size_t i = 0; i < AddressTable::COUNT; i++) {
		if (auto ptr = db.findAddressById(g_addressTable[i].ids[index])) {
			g_addressTable[i].address = reinterpret_cast<std::uintptr_t>(ptr);

			_VMESSAGE("%s: 0x%llx", names[i], g_addressTable[i].address - base);
		}
		else {
			_ERROR("ERROR: Offset for %s not found.", names[i]);
			success = false;
		}
	}

	return success;
}


IAddressLib::~IAddressLib()
{
	unload();
}

bool IAddressLib::load(const SkyrimVersion& version)
{
	unload();

	if (version.full < MAKE_EXE_VERSION(1, 6, 0)) {
		auto db = new AddressLib_1_5;

		loadedVersion = V1_5;
		loadedDb = db;

		return db->Load(version.major, version.minor, version.revision, 0);
	}
	else {
		auto db = new AddressLib_1_6;

		loadedVersion = V1_6;
		loadedDb = db;

		return db->Load(version.major, version.minor, version.revision, 0);
	}

	return false;
}

void* IAddressLib::findAddressById(unsigned long long id) const
{
	switch (loadedVersion) {
	case V1_5:
		return reinterpret_cast<AddressLib_1_5*>(loadedDb)->FindAddressById(id);
	case V1_6:
		return reinterpret_cast<AddressLib_1_6*>(loadedDb)->FindAddressById(id);
	}

	return nullptr;
}

bool IAddressLib::findIdByOffset(unsigned long long offset, unsigned long long& result) const
{
	switch (loadedVersion) {
	case V1_5:
		return reinterpret_cast<AddressLib_1_5*>(loadedDb)->FindIdByOffset(offset, result);
	case V1_6:
		return reinterpret_cast<AddressLib_1_6*>(loadedDb)->FindIdByOffset(offset, result);
	}

	return false;
}

bool IAddressLib::findOffsetById(unsigned long long id, unsigned long long& result) const
{
	switch (loadedVersion) {
	case V1_5:
		return reinterpret_cast<AddressLib_1_5*>(loadedDb)->FindOffsetById(id, result);
	case V1_6:
		return reinterpret_cast<AddressLib_1_6*>(loadedDb)->FindOffsetById(id, result);
	}

	return false;
}

void IAddressLib::unload()
{
	switch (loadedVersion) {
	case V1_5:
		delete reinterpret_cast<AddressLib_1_5*>(loadedDb);
		break;
	case V1_6:
		delete reinterpret_cast<AddressLib_1_6*>(loadedDb);
		break;
	}

	loadedDb = nullptr;
	loadedVersion = NONE;
}
