#include "CPlugin.h"

#include <filesystem>

extern LogNWNX* logger;

CPlugin::CPlugin(HINSTANCE hDLL, const NWNXCPlugin_InitInfo& initInfo)
{

	auto abiVersion = reinterpret_cast<uint32_t*> GetProcAddress(hDLL, "nwnxcplugin_abi_version");
	assert(abiVersion != nullptr);
	m_dll.abiVersion = *abiVersion;

	// Map functions
	if (m_dll.abiVersion == 1) {
		// Latest ABI version: just map functions as is
		// Note: older ABI version will need to provide custom function pointers to ensure
		// compatibility
		// clang-format off
		m_dll.newPlugin    = reinterpret_cast<decltype(m_dll.newPlugin)*>(   GetProcAddress(hDLL, "NWNXCPlugin_New"));
		m_dll.deletePlugin = reinterpret_cast<decltype(m_dll.deletePlugin)*>(GetProcAddress(hDLL, "NWNXCPlugin_Delete"));
		m_dll.getClassName = reinterpret_cast<decltype(m_dll.getClassName)*>(GetProcAddress(hDLL, "NWNXCPlugin_GetClassName"));
		m_dll.getVersion   = reinterpret_cast<decltype(m_dll.getVersion)*>(  GetProcAddress(hDLL, "NWNXCPlugin_GetVersion"));
		m_dll.getInt       = reinterpret_cast<decltype(m_dll.getInt)*>(      GetProcAddress(hDLL, "NWNXCPlugin_GetInt"));
		m_dll.setInt       = reinterpret_cast<decltype(m_dll.setInt)*>(      GetProcAddress(hDLL, "NWNXCPlugin_SetInt"));
		m_dll.getFloat     = reinterpret_cast<decltype(m_dll.getFloat)*>(    GetProcAddress(hDLL, "NWNXCPlugin_GetFloat"));
		m_dll.setFloat     = reinterpret_cast<decltype(m_dll.setFloat)*>(    GetProcAddress(hDLL, "NWNXCPlugin_SetFloat"));
		m_dll.getString    = reinterpret_cast<decltype(m_dll.getString)*>(   GetProcAddress(hDLL, "NWNXCPlugin_GetString"));
		m_dll.setString    = reinterpret_cast<decltype(m_dll.setString)*>(   GetProcAddress(hDLL, "NWNXCPlugin_SetString"));
		// clang-format on
	} else
		throw std::exception("Unsupported CPlugin ABI version " + std::to_string(m_dll.abiVersion));

	// Check mandatory definitions
	if (m_dll.newPlugin == nullptr)
		throw std::exception("Missing function: NWNXCPlugin_New");

	// Provide fallbacks if needed
	if (m_dll.getClassName == nullptr) {
		// Fallback to plugin file name
		auto dllName       = std::filesystem::path(initInfo.dll_path).stem();
		m_dll.getClassName = [dllName](void*) { return dllName.c_str(); };
	}

	// Initialize instance
	m_instancePtr = m_dll.newPlugin(initInfo);
	if (m_instancePtr == nullptr) {
		logger->Err("NWNXCPlugin_New returned null");
		return;
	}
}

CPlugin::~CPlugin()
{
	if (m_dll.deletePlugin)
		m_dll.deletePlugin(m_instancePtr);
}
std::string
CPlugin::GetClassName() const
{
	return std::string(m_dll.getClassName(m_instancePtr));
}
std::string
CPlugin::GetVersion() const
{
	if (m_dll.getVersion != nullptr)
		return std::string(m_dll.getVersion());
	return "unknown";
}

// bool CPlugin::Delete() {
// 	try {
// 		if (CallDelete == nullptr) {
// 			return false;
// 		}

// 		return CallDelete(plugin);
// 	} catch (const std::exception& exception) {
// 		logger->Warn("  * Calling Delete() failed: %s", exception.what());
// 	}

// 	return false;
// }

// std::string CPlugin::GetPluginId() {
// 	std::stringstream ss;
// 	ss << pluginName_ << "@" << pluginVersion_;

// 	return ss.str();
// }

// bool CPlugin::Ready() {
// 	return pluginName_ != nullptr;
// }

// bool CPlugin::Test(char* pluginName) {
// 	return strcmp(pluginName, pluginName) == 0;
// }

// void CPlugin::SetPluginName() {
// 	try {
// 		if (CallGetPluginName == nullptr) {
// 			return;
// 		}

// 		logger->Info("  * Getting C plugin name");
// 		pluginName_ = CallGetPluginName();
// 	} catch (const std::exception& exception) {
// 		logger->Warn("  * Calling SetPluginName() failed: %s", exception.what());
// 	}
// }

// void CPlugin::SetPluginVersion() {
// 	try {
// 		if (CallGetPluginVersion == nullptr) {
// 			return;
// 		}

// 		logger->Info("  * Getting C plugin version");
// 		pluginVersion_ = CallGetPluginVersion();
// 	} catch (const std::exception& exception) {
// 		logger->Warn("  * Calling SetPluginVersion() failed: %s", exception.what());
// 	}
// }

int
CPlugin::GetInt(const char* sFunction, const char* sParam1, int nParam2)
{
	if (m_dll.getInt) {
		return m_dll.getInt(m_instancePtr, sFunction, sParam1, nParam2);
	}
	return 0;
}

void
CPlugin::SetInt(const char* sFunction, const char* sParam1, int nParam2, int nValue)
{
	if (m_dll.setInt) {
		m_dll.setInt(m_instancePtr, sFunction, sParam1, nParam2, nValue);
	}
}

float
CPlugin::GetFloat(const char* sFunction, const char* sParam1, int nParam2)
{
	if (m_dll.getFloat) {
		return m_dll.getFloat(m_instancePtr, sFunction, sParam1, nParam2);
	}
	return 0.0;
}

void
CPlugin::SetFloat(const char* sFunction, const char* sParam1, int nParam2, float fValue)
{
	if (m_dll.setFloat) {
		m_dll.setFloat(m_instancePtr, sFunction, sParam1, nParam2, fValue);
	}
}

const char*
CPlugin::GetString(const char* sFunction, const char* sParam1, int nParam2)
{
	if (m_dll.getString) {
		return m_dll.getString(m_instancePtr, sFunction, sParam1, nParam2);
	}
	return nullptr;
}

void
CPlugin::SetString(const char* sFunction, const char* sParam1, int nParam2, char* sValue)
{
	if (m_dll.setString) {
		m_dll.setString(m_instancePtr, sFunction, sParam1, nParam2, sValue);
	}
}
