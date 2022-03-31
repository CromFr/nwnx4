#ifndef NWNX4_CPLUGIN_H
#define NWNX4_CPLUGIN_H

#include <windows.h>
#include "../misc/log.h"


class CPlugin {
public:
	struct NWNXCPlugin_InitInfo {
	    const char* dll_path; // plugin dll path
	    const char* nwnx_path; // nwnx4 executable path
	    const char* nwn2server_path; // nwn2 install dir

	    // const char* home_path; // nwn2 home path (i.e. by default Documents/Neverwinter Nights 2)
	    // const char* servervault_path; // server vault path if enabled
	};

	CPlugin(HINSTANCE hDLL, const NWNXCPlugin_InitInfo& initInfo);
	~CPlugin();

	bool GetIsLoaded() const {
		return m_instancePtr != nullptr;
	}

	std::string GetClassName() const;
	std::string GetVersion() const;

	int GetInt(const char* sFunction, const char* sParam1, int nParam2);
	void SetInt(const char* sFunction, const char* sParam1, int nParam2, int nValue);
	float GetFloat(const char* sFunction, const char* sParam1, int nParam2);
	void SetFloat(const char* sFunction, const char* sParam1, int nParam2, float fValue);
	void GetString(const char* sFunction, const char* sParam1, int nParam2, char* result, size_t resultSize);
	void SetString(const char* sFunction, const char* sParam1, int nParam2, char* sValue);
private:
	void* m_instancePtr = nullptr;

	// bool Delete();  // Called on destructor
	// void SetPluginName();
	// void SetPluginVersion();

	// CallGetPluginName_ CallGetPluginName;
	// CallGetPluginVersion_ CallGetPluginVersion;


	struct {
		uint32_t abiVersion;
		std::function<void*(NWNXCPlugin_InitInfo info)> newPlugin;
		std::function<void(void* cplugin)> deletePlugin;
		std::function<const char* (void* cplugin)> getClassName;
		std::function<const char* ()> getVersion;
		// clang-format off
		std::function<int32_t(void* cplugin, const char* sFunction, const char* sParam1, int32_t nParam2)> getInt;
		std::function<void   (void* cplugin, const char* sFunction, const char* sParam1, int32_t nParam2, int32_t nValue)> setInt;
		std::function<float  (void* cplugin, const char* sFunction, const char* sParam1, int32_t nParam2)> getFloat;
		std::function<void   (void* cplugin, const char* sFunction, const char* sParam1, int32_t nParam2, float fValue)> setFloat;
		std::function<void   (void* cplugin, const char* sFunction, const char* sParam1, int32_t nParam2, char* result, size_t resultSize)> getString;
		std::function<void   (void* cplugin, const char* sFunction, const char* sParam1, int32_t nParam2, const char* sValue)> setString;
		// clang-format on
	} m_dll;
};

#endif //NWNX4_CPLUGIN_H
