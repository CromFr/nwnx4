#pragma once

#include <stdint.h>

extern "C" {

#define NWNX_MAX_BUFFER 64*1024

struct NWNXCPlugin_InitInfo {
    const char* dll_path; // plugin dll path
    const char* nwnx_path; // nwnx4 executable path
    const char* nwn2server_path; // nwn2 install dir

    // const char* home_path; // nwn2 home path (i.e. by default Documents/Neverwinter Nights 2)
    // const char* servervault_path; // server vault path if enabled
};

// Plugin ABI version for nwnx4 compatibility
__declspec(dllexport)
extern uint32_t nwnxcplugin_abi_version;

__declspec(dllexport) __stdcall
void*       NWNXCPlugin_New(NWNXCPlugin_InitInfo info); // **Must** be defined
__declspec(dllexport) __stdcall
bool        NWNXCPlugin_Delete(void* cplugin); // useable? currently plugins are never destroyed

__declspec(dllexport) __stdcall
const char* NWNXCPlugin_GetClassName(void* cplugin); // plugin class name
__declspec(dllexport) __stdcall
const char* NWNXCPlugin_GetVersion(); // plugin version string

__declspec(dllexport) __stdcall
int32_t     NWNXCPlugin_GetInt(void* cplugin,    const char* sFunction, const char* sParam1, int32_t nParam2);
__declspec(dllexport) __stdcall
void        NWNXCPlugin_SetInt(void* cplugin,    const char* sFunction, const char* sParam1, int32_t nParam2, int32_t nValue);
__declspec(dllexport) __stdcall
float       NWNXCPlugin_GetFloat(void* cplugin,  const char* sFunction, const char* sParam1, int32_t nParam2);
__declspec(dllexport) __stdcall
void        NWNXCPlugin_SetFloat(void* cplugin,  const char* sFunction, const char* sParam1, int32_t nParam2, float fValue);
__declspec(dllexport) __stdcall
void        NWNXCPlugin_GetString(void* cplugin, const char* sFunction, const char* sParam1, int32_t nParam2, char* result, size_t resultSize);
__declspec(dllexport) __stdcall
void        NWNXCPlugin_SetString(void* cplugin, const char* sFunction, const char* sParam1, int32_t nParam2, const char* sValue);
}
