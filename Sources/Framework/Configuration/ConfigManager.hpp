#ifndef PROJECTWAAAGH_CONFIGMANAGER_HPP
#define PROJECTWAAAGH_CONFIGMANAGER_HPP

#include "Defines.hpp"

#include <string>
#include <vector>

class PRW_FRAMEWORK_API ConfigManager {
public:

	ConfigManager() = default;
	ConfigManager(ConfigManager const&) = delete;
	ConfigManager& operator=(ConfigManager const&) = delete;
	~ConfigManager() = default;

	bool Initialize(std::string configFile, std::vector<std::string> configArgs, std::string& configError);
	bool InitializeAdditional(std::string configFile, bool keepOnReload, std::string& configError);

	static ConfigManager* Instance();

	bool Reload(std::vector<std::string>& configError);

	std::string GetStringDefault(std::string const& configName, const std::string& defaultValue, bool quiet = false) const;
	bool GetBoolDefault(std::string const& configName, bool defaultValue, bool quiet = false) const;
	int32 GetInDefault(std::string const& configName, int32 defaultValue, bool quiet = false) const;
	int64 GetInt64Default(std::string const& configName, int64 defaultValue, bool quiet = false) const;
	float GetFloatDefault(std::string const& configName, float defaultValue, bool quiet = false) const;

	std::string const& GetFilename();
	std::vector<std::string> const& GetArguments() const;
	std::vector<std::string> GetKeysByString(std::string const& configString);

private:

	template<class T>
	T GetValueDefault(std::string const& configName, T defaultValue, bool quiet) const;
};

#define sConfigManager ConfigManager::Instance()

#endif // !PROJECTWAAAGH_CONFIGMANAGER_HPP
