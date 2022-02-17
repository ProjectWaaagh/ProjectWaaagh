#include "Banner.hpp"
#include "GitRevision.hpp"
#include "StringFormat.hpp"

void ProjectWaaagh::Banner::Show(char const* applicationName, void (*log)(char const* text), void(*logExtraInfo)()) {
	log(ProjectWaaagh::StringFormat("%s (%s)", GitRevision::GetFullVersion(), applicationName).c_str());
	log("<Ctrl-C> to stop.");
	log(" ___          _        _ __      __                   _    ");
	log("| _ \\_ _ ___ (_)___ __| |\\ \\    / /_ _ __ _ __ _ __ _| |_  ");
	log("|  _/ '_/ _ \\| / -_) _|  _\\ \\/\\/ / _` / _` / _` / _` | ' \\ ");
	log("|_| |_| \\___// \\___\\__|\\__|\\_/\\_/\\__,_\\__,_\\__,_\\__, |_||_|");
	log("           |__/                                 |___/      ");
	log("https://Project-Waaagh.net");
	
	if (logExtraInfo)
		logExtraInfo();
}
