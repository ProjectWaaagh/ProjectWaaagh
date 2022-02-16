#ifndef __REVISION_DATA_HPP__
#define __REVISION_DATA_HPP__

#define _HASH						"@revHash@"
#define _DATE						"@revDate@"
#define _BRANCH						"@revBranch@"
#define _CMAKE_COMMAND				R"(@CMAKE_COMMAND@)"
#define _CMAKE_VERSION				R"(@CMAKE_VERSION@)"
#define _CMAKE_HOST_SYSTEM			R"(@CMAKE_HOST_SYSTEM_NAME@ @CMAKE_HOST_SYSTEM_VERSION@)"
#define _SOURCE_DIRECTORY			R"(@CMAKE_SOURCE_DIR@)"
#define _BUILD_DIRECTORY			R"(@BUILDDIR@)"
#define _MYSQL_EXECUTABLE			R"(@MYSQL_EXECUTABLE@)"
#define _FULL_DATABASE				"PRW_FULL_WORLD_VERSION.sql"
#define _HOTFIXES_DATABASE			"PRW_FULL_HOTFIXES_VERSION.sql"
#define VER_COMPANYNAME_STR			"ProjectWaaagh Developers"
#define VER_LEGALCOPYRIGHT_STR		"(c) 2021 - 2022 ProjectWaaagh"
#define VER_FILEVERSION				0,0,0
#define VER_FILEVERSION_STR			"@revHash@ @revDate@ (@revBanch@ branch)"
#define VER_PRODUCTVERSION			VER_FILEVERSION
#define VER_PRODUCTVERSION_STR		VER_FILEVERSION_STR

#endif //__REVISION_DATA_HPP__