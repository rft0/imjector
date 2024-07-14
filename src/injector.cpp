#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <Shlwapi.h>

#include "injector.h"
#include "globals.h"

InjectionMethod Injector::currentMethod = INJECT_LOADLIBRARY;
ProcessData Injector::currentProcess = { "", 0 };

bool Injector::bStatus = false;

char Injector::szLastError[256] = "";
char Injector::szDllPath[MAX_PATH] = "";
char Injector::szDllName[MAX_PATH] = "";

std::vector<ProcessData> Injector::vecFilteredProcs = {};
std::vector<ProcessData> Injector::vecProcs = {};

void Injector::UpdateFilteredProcessList(const char* filter) {
    vecFilteredProcs.clear();

    for (ProcessData proc : vecProcs) {
        if (filter[0] == '\0' || StrStrIA(proc.szProcessName, filter))
            vecFilteredProcs.push_back(proc);
    }
}

void Injector::UpdateProcessList() {
    vecProcs.clear();

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	PROCESSENTRY32W entry{};
	entry.dwSize = sizeof(PROCESSENTRY32W);

    while (Process32NextW(hSnap, &entry)) {
        ProcessData data;
        
        if (WideCharToMultiByte(CP_UTF8, 0, entry.szExeFile, -1, data.szProcessName, sizeof(data.szProcessName), NULL, NULL) == 0)
            data.szProcessName[0] = '\0';

        data.dwPid = entry.th32ProcessID;

        HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, data.dwPid);
        if (hProc) {
            BOOL isWow64 = FALSE;
            IsWow64Process(hProc, &isWow64);
            data.arch = isWow64 ? ARCH_X86 : ARCH_X64;
            CloseHandle(hProc);
        }
        else
            data.arch = ARCH_X86;

        vecProcs.push_back(data);
    }

    CloseHandle(hSnap);
}


bool Injector::OpenFileDialog() {
    OPENFILENAMEW ofn;
    wchar_t szFile[MAX_PATH] = L"";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"DLLs (*.dll)\0*.dll\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;

    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        memset(szDllPath, 0, sizeof(szDllPath));

        if (WideCharToMultiByte(CP_UTF8, 0, ofn.lpstrFile, -1, szDllPath, sizeof(szDllPath), NULL, NULL) == 0)
            szDllPath[0] = '\0';

        char* p = strrchr(szDllPath, '\\');
        p ? strcpy_s(szDllName, p + 1) : strcpy_s(szDllName, szDllPath);
        return true;
    }

    return false;
}

const char* Injector::GetError() {
    return szLastError;
}

void Injector::SetError(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsprintf_s(szLastError, sizeof(szLastError), fmt, args);
    va_end(args);
}

bool Injector::Inject() {
    if (currentProcess.szProcessName[0] == '\0' || szDllPath[0] == '\0')
        INJ_ASSERT("You must select process and DLL First.");

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, currentProcess.dwPid);
    if (hProc == NULL)
        INJ_ASSERT("Process is not running anymore.");

    if (GetFileAttributesA(szDllPath) == INVALID_FILE_ATTRIBUTES) {
        CloseHandle(hProc);
        INJ_ASSERT("DLL file does not exist.");
    }

    szLastError[0] = '\0';
    bStatus = true;

    if (currentMethod == INJECT_LOADLIBRARY) {
        if (!InjectLoadLibrary(hProc)) {
            CloseHandle(hProc);
            return false;
        }
    } else if (currentMethod == INJECT_MANUALMAP) {
        HANDLE hFile = CreateFileA(szDllPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
            INJ_ASSERT("Failed to open DLL! Error Code: %lx", GetLastError());
        
        DWORD dwFileSize = GetFileSize(hFile, NULL);
        if (dwFileSize == INVALID_FILE_SIZE) {
            CloseHandle(hFile);
            INJ_ASSERT("Failed to get DLL size! Error Code: %lx", GetLastError());
        }

        BYTE* pSrcData = (BYTE*)malloc(dwFileSize);
        if (pSrcData == NULL) {
            CloseHandle(hFile);
            INJ_ASSERT("Failed to allocate memory for DLL! Error Code: %lx", GetLastError());
        }

        DWORD dwBytesRead = 0;
        if (!ReadFile(hFile, pSrcData, dwFileSize, &dwBytesRead, NULL) || dwBytesRead != dwFileSize) {
            free(pSrcData);
            CloseHandle(hFile);
            INJ_ASSERT("Failed to read DLL! Error Code: %lx", GetLastError());
        }

        CloseHandle(hFile);

        if (!InjectManualMap(hProc, pSrcData)) {
            free(pSrcData);
            CloseHandle(hProc);
            return false;
        }

        free(pSrcData);
    }

    CloseHandle(hProc);
    return true;
}


bool Injector::InjectLoadLibrary(HANDLE hProc) {
    void *injectDllPathRemote = VirtualAllocEx(hProc, 0x00, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (injectDllPathRemote == NULL)
        INJ_ASSERT("Failed to allocate memory in remote process! Error Code: %lx", GetLastError());

    if (!WriteProcessMemory(hProc, injectDllPathRemote, szDllPath, strlen(szDllPath) + 1, 0))
        INJ_ASSERT("Failed to write to remote process! Error Code: %lx", GetLastError());

    HANDLE hRemoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, injectDllPathRemote, 0, NULL);
    if (hRemoteThread != NULL && hRemoteThread != INVALID_HANDLE_VALUE)
        CloseHandle(hRemoteThread);
    else
        INJ_ASSERT("Failed to create remote thread! Error Code: %lx", GetLastError());

    return true;
}