#include <thread>
#include <vector>

#include "../imgui/imgui.h"

#include "globals.h"

#include "gui.h"
#include "injector.h"

bool procPopup = false;

ProcessData selectedProc{};

char inputProcBuffer[257] = "";

std::string dllName;
std::string dllPath;

std::vector <ProcessData> procs = injector::GetAllProcesses(inputProcBuffer);
void gui::Update() {
    // Weird Bug: If I set pos before ahead window becomes not moveable.
    //ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ g::WINDOW_WIDTH, 192 });
    ImGui::Begin("Injector", &g::isRunning, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);
    if (procPopup) {
        ImGui::SetNextWindowSize({ g::WINDOW_WIDTH, g::WINDOW_HEIGHT });
        ImGui::OpenPopup("Processes");
    }

    if (ImGui::BeginPopupModal("Processes", &procPopup, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {
        ImGui::Text("Select a process.");
        ImGui::Text("Filter:");
        ImGui::SameLine();
        if (ImGui::InputText("##ProcInput", inputProcBuffer, sizeof(inputProcBuffer))) {
            if (strlen(inputProcBuffer) > 256)
                inputProcBuffer[256] = '\0';

            procs = injector::GetAllProcesses(inputProcBuffer);
        }

        ImGui::BeginChild("Scrolling");
        for (ProcessData procd : procs)
            if (ImGui::MenuItem(procd.processName.c_str())) {
                selectedProc = procd;
                procPopup = false;
            }

        ImGui::EndChild();
        ImGui::EndPopup();
    }

    ImGui::Text("Process Name:");

    ImGui::Text("%s", selectedProc.processName.c_str());
    if (ImGui::Button("Select Process", { 240, 0 }))
        procPopup = true;

    ImGui::Text("DLL Name:");
    ImGui::Text("%s", dllName.c_str());
    if (ImGui::Button("Select DLL", { 240, 0 })) {
        injector::OpenFileDialog(dllPath);

        size_t sep = dllPath.find_last_of("/\\");
        if (sep != std::string::npos)
            dllName = dllPath.substr(sep + 1);
    }

    //! TODO: Add platform detection, warn if there is a platform mismatch between dll and application. (Ex: x64 app, x86 dll etc...)
    if (ImGui::Button("Inject", { 240, 32 })) {

        if (selectedProc.processName.empty() || dllPath.empty())
            MessageBox(GetActiveWindow(), "You must select process and DLL First.", "Warning!", MB_ICONWARNING);
        else {
            bool res = injector::Inject(selectedProc.processId, dllPath.c_str());
            if (res) MessageBox(GetActiveWindow(), "DLL injected successfully.", "Success!", MB_OK);
            else MessageBox(GetActiveWindow(), "Failed to inject DLL.", "Fail!", MB_ICONERROR);

            inputProcBuffer[0] = '\0';
            selectedProc = {};
            dllName = "";
            dllPath = "";
        }
    }

    ImGui::End();
}