#include "GitRevision.hpp"
#include "RevisionData.hpp"

char const* GitRevision::GetHash() { return _HASH; }
char const* GitRevision::GetDate() { return _DATE; }
char const* GitRevision::GetBranch() { return _BRANCH; }

char const* GitRevision::GetCMakeCommand() { return _CMAKE_COMMAND; }
char const* GitRevision::GetCMakeVersion() { return _CMAKE_VERSION; }
char const* GitRevision::GetHostOSVersion() { return _CMAKE_HOST_SYSTEM; }

char const* GitRevision::GetBuildDirectory() { return _BUILD_DIRECTORY; }
char const* GitRevision::GetSourceDirectory() { return _SOURCE_DIRECTORY; }

char const* GitRevision::GetMySQLExecutable() { return _MYSQL_EXECUTABLE; }
char const* GitRevision::GetFullDatabase() { return _FULL_DATABASE; }
char const* GitRevision::GetHotfixesDatabase() { return _HOTFIXES_DATABASE; }

#if PROJECTWAAAGH_PLATFORM == PROJECTWAAAGH_PLATFORM_WINDOWS
#	ifdef _WIN64
#		define PROJECTWAAAGH_PLATFORM_STR "Win64"
#	else
#		define PROJECTWAAAGH_PLATFORM_STR "Win32"
#	endif
#elif PROJECTWAAAGH_PLATFROM == PROJECTWAAAGH_PLATFROM_APPLE
#	define PROJECTWAAAGH_PLATFROM_STR "MaxOSX"
#elif PROJECTWAAAGH_PLATFROM == PROJECTWAAAGH_PLATFROM_INTEL
#	define PROJECTWAAAGH_PLATFROM_STR "Intel"
#else
#	define PROJECTWAAAGH_PLATFROM_STR "Unix"
#endif

#ifndef PROJECTWAAAGH_API_USE_DYNAMIC_LINKING
#	define PROJECTWAAAGH_LINKAGE_TYPE_STR "Static"
#else
#	define PROJECTWAAAGH_LINKAGE_TYPE_STR "Dynamic"
#endif // !PROJECTWAAAGH_API_USE_DYNAMIC_LINKING

char const* GitRevision::GetFullVersion() {
	return "ProjectWaaagh rev. " VER_PRODUCTVERSION_STR " (" PROJECTWAAAGH_PLATFORM_STR ", " _BUILD_DIRECTIVE ", " PROJECTWAAAGH_LINKAGE_TYPE_STR ")";
}
char const* GitRevision::GetCompanyNameStr() { return VER_COMPANYNAME_STR; }
char const* GitRevision::GetLegalCopyrightStr() { return VER_LEGALCOPYRIGHT_STR; }
char const* GitRevision::GetFileVersionStr() { return VER_FILEVERSION_STR; }
char const* GitRevision::GetProductVersionStr() { return VER_PRODUCTVERSION_STR; }
