#include "style.h"
#include "imgui/imgui.h"

void Style::LoadStyles() {
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 3;
    config.PixelSnapH = true;

    io.Fonts->AddFontDefault();
    ImFont* pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 16.0f, &config);
    io.FontDefault = pFont;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowMinSize        = ImVec2( 256, 32 );
    style.FramePadding         = ImVec2( 4, 2 );
    style.ItemSpacing          = ImVec2( 6, 2 );
    style.ItemInnerSpacing     = ImVec2( 6, 4 );
    style.Alpha                = 0.95f;
    style.WindowRounding       = 4.0f;
    style.FrameRounding        = 2.0f;
    style.IndentSpacing        = 6.0f;
    style.ItemInnerSpacing     = ImVec2( 2, 4 );
    style.ColumnsMinSpacing    = 50.0f;
    style.GrabMinSize          = 14.0f;
    style.GrabRounding         = 16.0f;
    style.ScrollbarSize        = 12.0f;
    style.ScrollbarRounding    = 16.0f;

    style.Colors[ImGuiCol_Text]                  = ImVec4(0.86f, 0.93f, 0.89f, 0.85f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.22f, 0.27f, 0.60f); //
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.47f, 0.77f, 0.83f, 0.08f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.47f, 0.77f, 0.83f, 0.08f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.10f, 0.11f, 0.14f, 1.00f); // ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.10f, 0.11f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.10f, 0.11f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.09f, 0.15f, 0.16f, 0.80f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.09f, 0.15f, 0.16f, 0.60f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.47f, 0.77f, 0.83f, 0.08f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.47f, 0.77f, 0.83f, 0.08f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.47f, 0.77f, 0.83f, 0.08f);
    style.Colors[ImGuiCol_Header]                = ImVec4(0.47f, 0.77f, 0.83f, 0.08f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.47f, 0.77f, 0.83f, 0.08f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.47f, 0.77f, 0.83f, 0.08f);
    style.Colors[ImGuiCol_Separator]             = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.47f, 0.77f, 0.83f, 0.08f);
    style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.47f, 0.77f, 0.83f, 0.08f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.47f, 0.77f, 0.83f, 0.08f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.47f, 0.77f, 0.83f, 0.08f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.47f, 0.77f, 0.83f, 0.08f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.47f, 0.77f, 0.83f, 0.08f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.15f, 0.16f, 0.17f, 0.80f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
}