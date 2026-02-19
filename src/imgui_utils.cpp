#include "../include/imgui_utils.h"
#include "../libs/imgui/imgui.h"
#include <vector>

void ApplyNeonStyle() {
  ImGuiStyle &style = ImGui::GetStyle();

  // Rounding
  style.WindowRounding = 12.0f;
  style.ChildRounding = 12.0f;
  style.FrameRounding = 12.0f;
  style.GrabRounding = 12.0f;
  style.PopupRounding = 12.0f;
  style.ScrollbarRounding = 12.0f;
  style.TabRounding = 12.0f;

  // Colors
  ImVec4 *colors = style.Colors;

  ImVec4 color_bg = ImVec4(0.05f, 0.05f, 0.08f, 1.00f);
  ImVec4 color_text = ImVec4(0.90f, 0.90f, 0.95f, 1.00f);
  ImVec4 color_primary = ImVec4(1.00f, 0.00f, 0.70f, 1.00f);   // Neon Pink
  ImVec4 color_secondary = ImVec4(0.00f, 1.00f, 1.00f, 1.00f); // Neon Cyan
  ImVec4 color_accent = ImVec4(0.50f, 0.00f, 1.00f, 1.00f);    // Deep Purple

  colors[ImGuiCol_Text] = color_text;
  colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_WindowBg] = color_bg;
  colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.10f, 1.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.12f, 0.94f);
  colors[ImGuiCol_Border] = color_primary;
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.20f, 0.54f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.25f, 0.40f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.30f, 0.67f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.00f, 0.15f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
  colors[ImGuiCol_CheckMark] = color_secondary;
  colors[ImGuiCol_SliderGrab] = color_secondary;
  colors[ImGuiCol_SliderGrabActive] = color_primary;
  colors[ImGuiCol_Button] = color_accent;
  colors[ImGuiCol_ButtonHovered] = color_primary;
  colors[ImGuiCol_ButtonActive] = color_secondary;
  colors[ImGuiCol_Header] = ImVec4(0.30f, 0.00f, 0.50f, 0.31f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.00f, 0.50f, 0.80f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.00f, 0.50f, 1.00f);
  colors[ImGuiCol_Separator] = color_secondary;
  colors[ImGuiCol_SeparatorHovered] = color_primary;
  colors[ImGuiCol_SeparatorActive] = color_primary;
  colors[ImGuiCol_ResizeGrip] = color_secondary;
  colors[ImGuiCol_ResizeGripHovered] = color_primary;
  colors[ImGuiCol_ResizeGripActive] = color_primary;
  colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.00f, 0.25f, 0.86f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.00f, 0.60f, 0.80f);
  colors[ImGuiCol_TabActive] = color_primary;
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
  colors[ImGuiCol_PlotLines] = color_secondary;
  colors[ImGuiCol_PlotLinesHovered] = color_primary;
  colors[ImGuiCol_PlotHistogram] = color_secondary;
  colors[ImGuiCol_PlotHistogramHovered] = color_primary;
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
  colors[ImGuiCol_DragDropTarget] = color_primary;
  colors[ImGuiCol_NavHighlight] = color_secondary;
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void HelpMarker(const char *desc) {
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}
