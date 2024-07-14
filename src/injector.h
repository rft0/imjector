#pragma once

#include <string>
#include <vector>
#include <Windows.h>

#define INJ_ASSERT(fmt, ...) { Injector::SetError(fmt, __VA_ARGS__); return false; }
#ifdef _WIN64
using f_RtlAddFunctionTable = BOOL(WINAPIV*)(PRUNTIME_FUNCTION FunctionTable, DWORD EntryCount, DWORD64 BaseAddress);
#endif

using f_LoadLibraryA = HINSTANCE(WINAPI*)(const char* lpLibFilename);
using f_GetProcAddress = FARPROC(WINAPI*)(HMODULE hModule, LPCSTR lpProcName);
using f_DLL_ENTRY_POINT = BOOL(WINAPI*)(void* hDll, DWORD dwReason, void* pReserved);


enum InjectionMethod {
	INJECT_LOADLIBRARY = 0,
	INJECT_MANUALMAP
};

enum Arch {
	ARCH_X86 = 0,
	ARCH_X64
};

struct ProcessData {
	char szProcessName[MAX_PATH];
	DWORD dwPid;
	Arch arch;
};

struct MMData
{
	f_LoadLibraryA pLoadLibraryA;
	f_GetProcAddress pGetProcAddress;
#ifdef _WIN64
	f_RtlAddFunctionTable pRtlAddFunctionTable;
#endif
	BYTE* pbase;
	HINSTANCE hMod;
	DWORD fdwReasonParam;
	LPVOID reservedParam;
	BOOL SEHSupport;
};


class Injector {
public:
	static InjectionMethod currentMethod;
	static ProcessData currentProcess;

	static bool bStatus;

	static char szDllPath[MAX_PATH];
	static char szDllName[MAX_PATH];

	static std::vector<ProcessData> vecFilteredProcs;
	static std::vector<ProcessData> vecProcs;

	static void UpdateFilteredProcessList(const char* filter = "");
	static void UpdateProcessList();

	static bool OpenFileDialog();
	static bool Inject();

	static const char* GetError();
	static void SetError(const char* fmt, ...);

private:
	static char szLastError[256];

	static bool InjectLoadLibrary(HANDLE hProc);
	static bool InjectManualMap(HANDLE hProc, BYTE* pSrcData);
};