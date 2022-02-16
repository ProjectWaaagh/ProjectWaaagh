#ifndef __GITREVISION_HPP__
#define __GITREVISION_HPP__

#include "Defines.hpp"
namespace GitRevision {

	PRW_FRAMEWORK_API char const* GetHash();
	PRW_FRAMEWORK_API char const* GetDate();
	PRW_FRAMEWORK_API char const* GetBranch();

	PRW_FRAMEWORK_API char const* GetCMakeCommand();
	PRW_FRAMEWORK_API char const* GetCMakeVersion();
	PRW_FRAMEWORK_API char const* GetHostOSVersion();

	PRW_FRAMEWORK_API char const* GetSourceDirectory();
	PRW_FRAMEWORK_API char const* GetBuildDirectory();

	PRW_FRAMEWORK_API char const* GetMySQLExecutable();
	PRW_FRAMEWORK_API char const* GetFullDatabase();
	PRW_FRAMEWORK_API char const* GetHotfixesDatabase();

	PRW_FRAMEWORK_API char const* GetFullVersion();
	PRW_FRAMEWORK_API char const* GetCompanyNameStr();
	PRW_FRAMEWORK_API char const* GetLegalCopyrightStr();
	PRW_FRAMEWORK_API char const* GetFileVersionStr();
	PRW_FRAMEWORK_API char const* GetProductVersionStr();
}

#endif //__GITREVISION_HPP__
