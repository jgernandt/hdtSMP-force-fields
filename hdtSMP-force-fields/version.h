#pragma once

constexpr unsigned long VERSION_MAJOR{ 0 };
constexpr unsigned long VERSION_MINOR{ 9 };
constexpr unsigned long VERSION_PATCH{ 0 };
constexpr unsigned long VERSION = (VERSION_MAJOR & 0xFF) << 24 | (VERSION_MINOR & 0xFF) << 16 | (VERSION_PATCH & 0xFF) << 8;

struct SkyrimVersion
{
	int major;
	int minor;
	int revision;
	int build;
	unsigned int full;
};
