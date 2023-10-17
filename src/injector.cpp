#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <vector>
#include <string>
#include <algorithm>

#include "injector.h"
#include "globals.h"

bool stristr(const char* haystack, const char* needle) {
    while (*haystack) {
        const char* hayPtr = haystack;
        const char* neePtr = needle;

        while (*hayPtr && *neePtr && tolower((unsigned char)*hayPtr) == tolower((unsigned char)*neePtr)) {
            hayPtr++;
            neePtr++;
        }

        if (!*neePtr) {
            return true;
        }

        haystack++;
    }

    return false;
}

std::vector<ProcessData> injector::GetAllProcesses(const char* filter) {
	std::vector<ProcessData> processData;

	HANDLE hProcessId = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	PROCESSENTRY32 entry{};
	entry.dwSize = sizeof(entry);

    char procName[256];

	while (Process32Next(hProcessId, &entry))
        if (strlen(filter) == 0 || stristr(entry.szExeFile, filter)) {
            snprintf(procName, sizeof(procName), "%s -> %lu", entry.szExeFile, entry.th32ProcessID);
		    processData.push_back(ProcessData{ procName, entry.th32ProcessID });
        }

	return processData;
}

bool injector::OpenFileDialog(std::string& selectedFilePath) {
    OPENFILENAME ofn;
    char szFile[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "DLLs (*.dll)\0*.dll\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;

    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&ofn)) {
        selectedFilePath = ofn.lpstrFile;
        return true;
    }

    return false;
}

bool injector::Inject(DWORD pid, const char* dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
    if (!hProcess)
        return false;

    LPVOID ntOpenFile = reinterpret_cast<LPVOID>(GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile"));
    if (ntOpenFile) {
        char originalBytes[5];
        memcpy(originalBytes, ntOpenFile, 5);
        WriteProcessMemory(hProcess, ntOpenFile, originalBytes, 5, NULL);
    }

    LPVOID allocatedMem = VirtualAllocEx(hProcess, NULL, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!allocatedMem)
        return false;

    if (!WriteProcessMemory(hProcess, allocatedMem, dllPath, sizeof(dllPath), NULL))
        return false;

    CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, allocatedMem, 0, 0);

    CloseHandle(hProcess);
    return true;
}