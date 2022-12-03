#include "pch.h"
#include "Config.h"
#include "ForceFieldManager.h"
#include "relocation.h"
#include "var_size.h"
#include "version.h"

using UInt32 = std::uint32_t;
using UInt64 = std::uint64_t;
#include "skse64/PluginAPI.h"
#include "PluginHelper.h"

constexpr const char* LOG_PATH{ "\\My Games\\Skyrim Special Edition\\SKSE\\HDT-SMP Force Fields.log" };
constexpr const char* CONFIG_PATH{ "My Games\\Skyrim Special Edition\\SKSE\\HDT-SMP Force Fields.ini" };

bool attachHooks();
bool loadConfig()
{
	PWSTR wpath;
	HRESULT res = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &wpath);
	if (SUCCEEDED(res)) {
		std::filesystem::path documents(wpath, std::filesystem::path::native_format);
		std::filesystem::path configPath(documents / CONFIG_PATH);

		_VMESSAGE("File path: %s", configPath.string().c_str());

		return jg::g_config.load(configPath);
	}
	else {
		_WARNING("Documents folder not found.\n");
		return false;
	}
}

class MyHelper
{
public:
	inline static hdt::PluginInterface::Version interfaceMin{ 1, 0, 0 };
	inline static hdt::PluginInterface::Version interfaceMax{ 2, 0, 0 };

	inline static hdt::PluginInterface::Version bulletMin{ hdt::PluginInterface::BULLET_VERSION };
	inline static hdt::PluginInterface::Version bulletMax{ hdt::PluginInterface::BULLET_VERSION.major + 1, 0, 0 };

	inline static hdt::PluginInterface* ifc = nullptr;

	static void onConnect(hdt::PluginInterface* smp)
	{
		ifc = smp;
	}

	static void skseCallback(SKSEMessagingInterface::Message* msg)
	{
		if (msg && msg->type == SKSEMessagingInterface::kMessage_DataLoaded) {
			if (ifc) {
				gLog.Outdent();
				_MESSAGE("Connection established.\n");

				_MESSAGE("Starting ForceFieldManager...");
				gLog.Indent();

				ifc->addListener(&jg::ForceFieldManager::get());

				gLog.Outdent();
				_MESSAGE("ForceFieldManager started.");

				gLog.Outdent();
				_MESSAGE("Initialisation complete.\n");
			}
			else {
				gLog.Outdent();
				_ERROR("Failed to connect to HDT-SMP.");

				gLog.Outdent();
				_MESSAGE("Initialisation failed.");
			}
		}
	}
};

static void reportVersion(const SKSEInterface& skse, SkyrimVersion& r_version)
{
	r_version.full = skse.runtimeVersion;
	r_version.major = GET_EXE_VERSION_MAJOR(r_version.full);
	r_version.minor = GET_EXE_VERSION_MINOR(r_version.full);
	r_version.revision = GET_EXE_VERSION_BUILD(r_version.full);
	r_version.build = GET_EXE_VERSION_SUB(r_version.full);

	_MESSAGE("Game version %d.%d.%d", r_version.major, r_version.minor, r_version.revision);
	_MESSAGE("SKSE version %d.%d.%d",
		GET_EXE_VERSION_MAJOR(skse.skseVersion),
		GET_EXE_VERSION_MINOR(skse.skseVersion),
		GET_EXE_VERSION_BUILD(skse.skseVersion));
	_MESSAGE("Plugin version %d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
}

extern "C" {

	__declspec(dllexport) SKSEPluginVersionData SKSEPlugin_Version = {
		SKSEPluginVersionData::kVersion,
		VERSION,
		"HDT-SMP Force Fields",
		"jgernandt",
		"",
		SKSEPluginVersionData::kVersionIndependentEx_NoStructUse,
		SKSEPluginVersionData::kVersionIndependent_AddressLibraryPostAE,
		{ 0 },
		0,
	};

	__declspec(dllexport) bool SKSEPlugin_Load(const SKSEInterface* skse)
	{
		assert(skse);

		gLog.OpenRelative(CSIDL_MYDOCUMENTS, LOG_PATH);
		gLog.SetPrintLevel(IDebugLog::kLevel_Message);
		gLog.SetLogLevel(IDebugLog::kLevel_Message);

		SkyrimVersion version;
		reportVersion(*skse, version);

		if (!loadConfig()) {
			_WARNING("Failed to load config file. Using default settings.\n");
		}

		auto logLevel = static_cast<IDebugLog::LogLevel>(std::clamp(jg::g_config.geti(jg::Config::LOG_LEVEL), 0, 5));
		gLog.SetPrintLevel(logLevel);
		gLog.SetLogLevel(logLevel);

		_MESSAGE("Initialising plugin...");
		gLog.Indent();

		_MESSAGE("Determining object layout...");
		gLog.Indent();
		SizeManager::init(version.full);
		gLog.Outdent();
		_MESSAGE("Object layout determined.\n");
		
		_MESSAGE("Creating address table...");
		gLog.Indent();
		if (initAddressTable(version)) {
			gLog.Outdent();
			_MESSAGE("Address table created.\n");
		}
		else {
			gLog.Outdent();
			_FATALERROR("Failed to create address table.");
			gLog.Outdent();
			_MESSAGE("Initialisation failed.");
			return false;
		}

		_MESSAGE("Attaching engine hooks...");
		gLog.Indent();
		if (attachHooks()) {
			gLog.Outdent();
			_MESSAGE("Engine hooks attached.\n");
		}
		else {
			gLog.Outdent();
			_FATALERROR("Failed to attach engine hooks.");
			gLog.Outdent();
			_MESSAGE("Initialisation failed.");
			return false;
		}

		_MESSAGE("Connecting to HDT-SMP...");
		gLog.Indent();
		hdt::PluginHelper<MyHelper>::tryConnect(skse);
		
		return true;
	}

	__declspec(dllexport) bool SKSEPlugin_Query(const SKSEInterface* skse, PluginInfo* info)
	{
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = SKSEPlugin_Version.name;
		info->version = SKSEPlugin_Version.pluginVersion;

		return true;
	}
};
