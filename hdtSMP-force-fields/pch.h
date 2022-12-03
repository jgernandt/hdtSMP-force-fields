#ifndef PCH_H
#define PCH_H

#include <algorithm>
#include <barrier>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <filesystem>
#include <list>
#include <map>
#include <mutex>
#include <random>
#include <semaphore>
#include <set>
#include <shared_mutex>
#include <string>
#include <thread>
#include <typeinfo>
#include <type_traits>
#include <vector>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <ShlObj.h>
#include <windows.h>

#include "detours.h"

#undef min
#undef max

#include "common/IDebugLog.h"
#include "skse64_common/skse_version.h"

#include "PluginAPI.h"

#include "BulletAPI.h"

#endif //PCH_H
