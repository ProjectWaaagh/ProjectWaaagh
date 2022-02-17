#include "ConfigManager.hpp"
#include "Log.hpp"
#include "StringConverter.hpp"
#include "Utilities.hpp"

#include <boost/property_tree/ini_parser.hpp>
#include <algorithm>
#include <memory>
#include <mutex>

namespace propertyTree = boost::property_tree;

namespace{
	
	std::string m_ConfigFile;
	std::vector<std::string> m_AdditionalFiles;
	std::vector<std::string> m_ConfigArgs;
	propertyTree::ptree m_Config;
	std::mutex m_ConfigLock;

	bool LoadConfig(std::string const& configFile, propertyTree::ptree& configTree, std::string& errorString) {
		try {
			propertyTree::ini_parser::read_ini(configFile, configTree);

			/***
			 * Check if the configFile is empty.
			 * and return if it an empty file.
			 */
			if (configTree.empty()) {
				errorString = "Empty configuration file (" + configFile + ")";
				return false;
			}
		}
		/***
		 * Catch errors during reading config file into given errorString 
		 */
		catch (propertyTree::ini_parser::ini_parser_error const& e) {
			if (e.line() == 0)
				errorString = e.message() + " (" + e.filename() + ")";
			else
				errorString = e.message() + " (" + e.filename() + ":" + std::to_string(e.line()) + ")";
			
			return false;
		}

		/***
		 * Anythin is fine also true
		 */
		return true;
	}
}

/***
  Description:
  Initialize a configFile to use it in other applications.

  @param configFile Name of the configuration file.
  @param configArgs Configuration arguments.
  @param configError Configuration error return string.
  @returns true if config file loaded success.
 ***/
bool ConfigManager::Initialize(std::string configFile, std::vector<std::string> configArgs, std::string& configError) {
	
	std::lock_guard<std::mutex> lock(m_ConfigLock);

	// Move Config file and args into memberHolder
	m_ConfigFile = std::move(configFile);
	m_ConfigArgs = std::move(configArgs);

	// Create a new configTree
	propertyTree::ptree configTree;

	// Check if config loaded corectly into configTree
	if (!LoadConfig(m_ConfigFile, configTree, configError))
		return false;

	m_Config = configTree.begin()->second;

	return true;
}

bool ConfigManager::InitializeAdditional(std::string configFile, bool keepOnReload, std::string& configError) {
	
	propertyTree::ptree configTree;
	if (!LoadConfig(configFile, configTree, configError))
		return false;

	for (propertyTree::ptree::value_type const& child : configTree.begin()->second)
		m_Config.put_child(propertyTree::ptree::path_type(child.first, '/'), child.second);

	if (keepOnReload)
		m_AdditionalFiles.emplace_back(std::move(configFile));

	return true;
}

ConfigManager* ConfigManager::Instance() {
	
	static ConfigManager Instance;
	return &Instance;
}

bool ConfigManager::Reload(std::vector<std::string>& configError) {
	
	std::string errorString;
	if (!Initialize(m_ConfigFile, std::move(m_ConfigArgs), errorString))
		configError.push_back(std::move(errorString));

	for (std::string const& additionalFile : m_AdditionalFiles)
		if (!InitializeAdditional(additionalFile, false, errorString))
			configError.push_back(std::move(errorString));

	return configError.empty();
}

template<class T>
T ConfigManager::GetValueDefault(std::string const& configName, T defaultValue, bool quiet) const {
	try {
		return m_Config.get<T>(propertyTree::ptree::path_type(configName, '/'));
	}
	catch (propertyTree::ptree_bad_path const&) {
		if (!quiet) {
			PRW_LOG_WARN("Server.Loading", "Missing configName %s in configFile %s, add \"%s = %s\" to this file",
				configName.c_str(), m_ConfigFile.c_str(), configName.c_str(), std::to_string(defaultValue).c_str());
		}
	}
	catch (propertyTree::ptree_bad_data const&) {
        PRW_LOG_ERROR("Server.Loading" "Bad value defines for configName %s in configFile %s, going to use %s as defaultValue.",
			configName.c_str(), m_ConfigFile.c_str(), std::to_string(defaultValue).c_str());
	}

	return defaultValue;
}

template<>
std::string ConfigManager::GetValueDefault<std::string>(std::string const& configName, std::string defaultValue, bool quiet) const {

	try {
		return m_Config.get<std::string>(propertyTree::ptree::path_type(configName, '/'));
	}
	catch (propertyTree::ptree_bad_path const&) {
		if (!quiet) {
			//TODO: Logging Service here
            PRW_LOG_WARN("Server.Loading" "Missing configName '%s' in configFile '%s', add '%s = %s' to this file.",
				configName.c_str(), m_ConfigFile.c_str(), configName.c_str(), defaultValue.c_str());
		}
	}
	catch (propertyTree::ptree_bad_data const&) {
		//TODO: Logging Service here
        PRW_LOG_ERROR("Server.Loading" "Bad Value defined for configName '%s' in configFile '%s', going to use '%s' as defaultValue.",
			configName.c_str(), m_ConfigFile.c_str(), defaultValue.c_str());
	}

	return defaultValue;
}

std::string ConfigManager::GetStringDefault(std::string const& configName, const std::string& defaultValue, bool quiet) const {

	std::string configValue = GetValueDefault(configName, defaultValue, quiet);
	configValue.erase(std::remove(configValue.begin(), configValue.end(), '"'), configValue.end());

	return configValue;
}

bool ConfigManager::GetBoolDefault(std::string const& configName, bool defaultValue, bool quiet) const {
	
	std::string configValue = GetValueDefault(configName, std::string(defaultValue ? "1" : "0"), quiet);
	configValue.erase(std::remove(configValue.begin(), configValue.end(), '"'), configValue.end());
	Optional<bool> boolValue = ProjectWaaagh::StringTo<bool>(configValue);
	if (boolValue)
		return *boolValue;
	else {
		//TODO: Logging Service here.
        PRW_LOG_ERROR("Server.Loading" "Bad value defined for configName '%s' in configFile '%s', going to use '%s' as defaultValue.",
			configName.c_str(), m_ConfigFile.c_str(), defaultValue ? "true" : "false");
		return defaultValue;
	}


}

int32 ConfigManager::GetInDefault(std::string const& configName, int32 defaultValue, bool quiet) const {
	return GetValueDefault(configName, defaultValue, quiet);
}

int64 ConfigManager::GetInt64Default(std::string const& configName, int64 defaultValue, bool quiet) const {
	return GetValueDefault(configName, defaultValue, quiet);
}

float ConfigManager::GetFloatDefault(std::string const& configName, float defaultValue, bool quiet) const {
	return GetValueDefault(configName, defaultValue, quiet);
}

std::string const& ConfigManager::GetFilename() {
	std::lock_guard<std::mutex> lock(m_ConfigLock);
	return m_ConfigFile;
}

std::vector<std::string> const& ConfigManager::GetArguments() const {
	return m_ConfigArgs;
}

std::vector<std::string> ConfigManager::GetKeysByString(std::string const& configString)
{
	std::lock_guard<std::mutex> lock(m_ConfigLock);

	std::vector<std::string> keys;
	for (propertyTree::ptree::value_type const& child : m_Config)
		if (child.first.compare(0, configString.length(), configString) == 0)
			keys.push_back(child.first);

	return keys;
}

