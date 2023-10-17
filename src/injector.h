#pragma once

#include <string>
#include <vector>
#include <Windows.h>

typedef struct ProcessData {
	std::string processName;
	DWORD processId;
} ProcessData;

namespace injector {
	std::vector<ProcessData> GetAllProcesses(const char* filter = "");
	bool OpenFileDialog(std::string& selectedFilePath);
	bool Inject(DWORD pid, const char* dllPath);
}