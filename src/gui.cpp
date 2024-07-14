
#include "imgui/imgui.h"
#include "globals.h"
#include "gui.h"
#include "injector.h"

#define IM_PAD(x, y) ImGui::Dummy(ImVec2(x, y))

static char szFilter[MAX_PATH] = "";
static bool bPopup = false;

void gui::Update() {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 8, 8 });
    ImGui::Begin(GWINDOW_TITLE, &g::isRunning, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);
    ImGui::PopStyleVar();

    ImGui::Text("Status: %s", Injector::bStatus? (Injector::GetError()[0] == '\0'? "Injected successfully." : "An error occurred.") : "");

    IM_PAD(0.0f, 4.0f);

    // ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    // IM_PAD(ImGui::GetWindowWidth(), 2.0f);
    // ImGui::PopStyleColor();

    // IM_PAD(0.0f, 4.0f);

    ImGui::Text("Process Name:");
    ImGui::Text("%s", Injector::currentProcess.szProcessName);
    ImGui::SameLine(ImGui::GetWindowWidth() - 72);
    if (ImGui::Button("Select##ProcessListButton", { 64, 0 })) {
        ImGui::OpenPopup("Process List");
        Injector::UpdateProcessList();
        Injector::UpdateFilteredProcessList(szFilter);
        bPopup = true;
    }

    IM_PAD(0.0f, 4.0f);

    ImGui::Text("DLL Name:");
    ImGui::Text("%s", Injector::szDllName);
    ImGui::SameLine(ImGui::GetWindowWidth() - 72);
    if (ImGui::Button("Select##OFNButton", { 64, 0 }))
        Injector::OpenFileDialog();

    IM_PAD(0.0f, 4.0f);

    ImGui::Text("Injection Method:");
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - 16);
    ImGui::Combo("##InjectionMethod", (int*)&Injector::currentMethod, "Load Library\0Manual Map\0");

    IM_PAD(0.0f, 8.0f);

    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - 16);
    ImGui::BeginDisabled(Injector::currentProcess.szProcessName[0] == '\0' || Injector::szDllPath[0] == '\0');
    if (ImGui::Button("Inject", { 240, 32 })) {
        if (!Injector::Inject())
            MessageBoxA(GetActiveWindow(), Injector::GetError(), "DLL Injection Error", MB_ICONERROR);

        szFilter[0] = '\0';
        Injector::currentProcess.szProcessName[0] = '\0';
        Injector::currentProcess.dwPid = 0;
        Injector::szDllName[0] = '\0';
        Injector::szDllPath[0] = '\0';
    }
    ImGui::EndDisabled();

    ImGui::SetNextWindowSize({ 400, 300 }, ImGuiCond_Appearing);
    if (ImGui::BeginPopupModal("Process List", &bPopup, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {
        IM_PAD(0.0f, 4.0f);

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Filter:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - 128);
        if (ImGui::InputText("##ProcInput", szFilter, sizeof(szFilter))) {
            if (strlen(szFilter) > MAX_PATH - 1)
                szFilter[MAX_PATH - 1] = '\0';

            Injector::UpdateFilteredProcessList(szFilter);
        }
        ImGui::SameLine(ImGui::GetWindowWidth() - 72);
        if (ImGui::Button("Refresh", { 64, 0 })) {
            Injector::UpdateProcessList();
            Injector::UpdateFilteredProcessList(szFilter);
        }

        IM_PAD(0.0f, 4.0f);

        if (ImGui::BeginChild("Scrolling")) {
            for (ProcessData proc : Injector::vecFilteredProcs) {
                if (ImGui::MenuItem(proc.szProcessName)) {
                    Injector::currentProcess = proc;
                    ImGui::CloseCurrentPopup();
                    bPopup = false;
                }

                ImGui::SameLine(ImGui::GetWindowWidth() - 48);
                ImGui::Text("%s", proc.arch == ARCH_X86 ? "x86" : "x64");
            }
            
            ImGui::EndChild();
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}