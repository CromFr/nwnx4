#include "CPlugin.h"

extern LogNWNX* logger;

CPlugin::CPlugin(HINSTANCE hDLL) {
	void* callDelete = GetProcAddress(hDLL, "NWNXCPlugin_Delete");
	if (callDelete != nullptr) {
		CallDelete = reinterpret_cast<CallDelete_>(callDelete);
		logger->Info("  * Bound NWNXCPlugin_Delete()");
	}

	void* callGetPluginName = GetProcAddress(hDLL, "NWNXCPlugin_GetPluginName");
	if (callGetPluginName != nullptr) {
		CallGetPluginName = reinterpret_cast<CallGetPluginName_>(callGetPluginName);
		logger->Info("  * Bound NWNXCPlugin_GetPluginName()");
	}

	void* callGetPluginVersion = GetProcAddress(hDLL, "NWNXCPlugin_GetPluginVersion");
	if (callGetPluginVersion != nullptr) {
		CallGetPluginVersion = reinterpret_cast<CallGetPluginVersion_>(callGetPluginVersion);
		logger->Info("  * Bound NWNXCPlugin_GetPluginVersion()");
	}

	void* callGetInt = GetProcAddress(hDLL, "NWNXCPlugin_GetInt");
	if (callGetInt != nullptr) {
		CallGetInt = reinterpret_cast<CallGetInt_>(callGetInt);
		logger->Info("  * Bound NWNXCPlugin_GetInt()");
	}

	void* callSetInt = GetProcAddress(hDLL, "NWNXCPlugin_SetInt");
	if (callSetInt != nullptr) {
		CallSetInt = reinterpret_cast<CallSetInt_>(callSetInt);
		logger->Info("  * Bound NWNXCPlugin_SetInt()");
	}

	void* callGetFloat = GetProcAddress(hDLL, "NWNXCPlugin_GetFloat");
	if (callGetFloat != nullptr) {
		CallGetFloat = reinterpret_cast<CallGetFloat_>(callGetFloat);
		logger->Info("  * Bound NWNXCPlugin_GetFloat()");
	}

	void* callSetFloat = GetProcAddress(hDLL, "NWNXCPlugin_SetFloat");
	if (callSetFloat != nullptr) {
		CallSetFloat = reinterpret_cast<CallSetFloat_>(callSetFloat);
		logger->Info("  * Bound NWNXCPlugin_SetFloat()");
	}

	void* callGetString = GetProcAddress(hDLL, "NWNXCPlugin_GetString");
	if (callGetString != nullptr) {
		CallGetString = reinterpret_cast<CallGetString_>(callGetString);
		logger->Info("  * Bound NWNXCPlugin_GetString()");
	}

	void* callSetString = GetProcAddress(hDLL, "NWNXCPlugin_SetString");
	if (callSetString != nullptr) {
		CallSetString = reinterpret_cast<CallSetString_>(callSetString);
		logger->Info("  * Bound NWNXCPlugin_SetString()");
	}

	SetPluginName();
	SetPluginVersion();
}

CPlugin::~CPlugin() {
	if (!Delete()) {
		logger->Err("  * Cannot delete C plugin for %s", GetPluginId().c_str());
	}
}

bool CPlugin::Delete() {
	try {
		if (CallDelete == nullptr) {
			return false;
		}

		return CallDelete(plugin);
	} catch (const std::exception& exception) {
		logger->Warn("  * Calling Delete() failed: %s", exception.what());
	}

	return false;
}

std::string CPlugin::GetPluginId() {
	std::stringstream ss;
	ss << pluginName_ << "@" << pluginVersion_;

	return ss.str();
}

bool CPlugin::Ready() {
	return pluginName_ != nullptr;
}

bool CPlugin::Test(char* pluginName) {
	return strcmp(pluginName, pluginName) == 0;
}

void CPlugin::SetPluginName() {
	try {
		if (CallGetPluginName == nullptr) {
			return;
		}

		logger->Info("  * Getting C plugin name");
		pluginName_ = CallGetPluginName();
	} catch (const std::exception& exception) {
		logger->Warn("  * Calling SetPluginName() failed: %s", exception.what());
	}
}

void CPlugin::SetPluginVersion() {
	try {
		if (CallGetPluginVersion == nullptr) {
			return;
		}

		logger->Info("  * Getting C plugin version");
		pluginVersion_ = CallGetPluginVersion();
	} catch (const std::exception& exception) {
		logger->Warn("  * Calling SetPluginVersion() failed: %s", exception.what());
	}
}

int CPlugin::GetInt(char* sFunction, char* sParam1, int nParam2) {
	try {
		if (CallGetInt == nullptr) {
			return 0;
		}

		return CallGetInt(plugin, sFunction, sParam1, nParam2);
	} catch (const std::exception& exception) {
		logger->Warn("  * Calling GetInt() failed: %s", exception.what());
	}

	return 0;
}

void CPlugin::SetInt(char* sFunction, char* sParam1, int nParam2, int nValue) {
	try {
		if (CallSetInt == nullptr) {
			return;
		}

		CallSetInt(plugin, sFunction, sParam1, nParam2, nValue);
	} catch (const std::exception& exception) {
		logger->Warn("  * Calling SetInt() failed: %s", exception.what());
	}
}

float CPlugin::GetFloat(char* sFunction, char* sParam1, int nParam2) {
	try {
		if (CallGetFloat == nullptr) {
			return 0.0;
		}

		return CallGetFloat(plugin, sFunction, sParam1, nParam2);
	} catch (const std::exception& exception) {
		logger->Warn("  * Calling GetFloat() failed: %s", exception.what());
	}

	return 0.0;
}

void CPlugin::SetFloat(char* sFunction, char* sParam1, int nParam2, float fValue) {
	try {
		if (CallSetFloat == nullptr) {
			return;
		}

		CallSetFloat(plugin, sFunction, sParam1, nParam2, fValue);
	} catch (const std::exception& exception) {
		logger->Warn("  * Calling SetFloat() failed: %s", exception.what());
	}
}

char* CPlugin::GetString(char* sFunction, char* sParam1, int nParam2) {
	try {
		if (CallGetString == nullptr) {
			return nullptr;
		}

		return CallGetString(plugin, sFunction, sParam1, nParam2);
	} catch (const std::exception& exception) {
		logger->Warn("  * Calling GetString() failed: %s", exception.what());
	}

	return nullptr;
}

void CPlugin::SetString(char* sFunction, char* sParam1, int nParam2, char* sValue) {
	try {
		if (CallSetString == nullptr) {
			return;
		}

		CallSetString(plugin, sFunction, sParam1, nParam2, sValue);
	} catch (const std::exception& exception) {
		logger->Warn("  * Calling SetString() failed: %s", exception.what());
	}
}
