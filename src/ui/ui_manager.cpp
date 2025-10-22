#include "ui_manager.h"
#include "../backends/imgui_impl_glfw.h"
#include "../backends/imgui_impl_opengl3.h"
#include "../config/slider_config.h"
#include <imgui.h>
#include <iostream>
#include <cmath>
#include <limits>
#include <cfloat>

namespace ui {

namespace {
config::SliderConfig g_sliderConfig;
settings::SettingsManager g_settingsManager;

void applyCustomTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 14.0f;
    style.FrameRounding = 12.0f;
    style.GrabRounding = 12.0f;
    style.ChildRounding = 12.0f;
    style.ScrollbarRounding = 12.0f;
    style.FramePadding = ImVec2(12.0f, 8.0f);
    style.ItemSpacing = ImVec2(12.0f, 10.0f);
    style.WindowPadding = ImVec2(20.0f, 18.0f);

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg]       = ImVec4(0.08f, 0.09f, 0.13f, 0.94f);
    colors[ImGuiCol_ChildBg]        = ImVec4(0.09f, 0.10f, 0.15f, 0.92f);
    colors[ImGuiCol_PopupBg]        = ImVec4(0.10f, 0.12f, 0.18f, 0.98f);
    colors[ImGuiCol_Border]         = ImVec4(0.22f, 0.24f, 0.33f, 0.60f);
    colors[ImGuiCol_FrameBg]        = ImVec4(0.16f, 0.19f, 0.28f, 0.90f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.38f, 0.65f, 0.90f);
    colors[ImGuiCol_FrameBgActive]  = ImVec4(0.27f, 0.46f, 0.78f, 0.90f);
    colors[ImGuiCol_Button]         = ImVec4(0.22f, 0.38f, 0.65f, 0.85f);
    colors[ImGuiCol_ButtonHovered]  = ImVec4(0.27f, 0.46f, 0.78f, 0.90f);
    colors[ImGuiCol_ButtonActive]   = ImVec4(0.33f, 0.55f, 0.95f, 0.95f);
    colors[ImGuiCol_Header]         = ImVec4(0.24f, 0.40f, 0.69f, 0.86f);
    colors[ImGuiCol_HeaderHovered]  = ImVec4(0.30f, 0.50f, 0.90f, 0.86f);
    colors[ImGuiCol_HeaderActive]   = ImVec4(0.33f, 0.55f, 0.95f, 0.90f);
    colors[ImGuiCol_SliderGrab]     = ImVec4(0.33f, 0.55f, 0.95f, 0.85f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.40f, 0.62f, 1.0f, 0.90f);
    colors[ImGuiCol_Text]           = ImVec4(0.88f, 0.91f, 0.96f, 1.0f);
    colors[ImGuiCol_TextDisabled]   = ImVec4(0.50f, 0.56f, 0.67f, 1.0f);
}
}

bool UIManager::init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    applyCustomTheme();

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
        std::cerr << "ImGui GLFW backend init failed." << std::endl;
        return false;
    }
    if (!ImGui_ImplOpenGL3_Init("#version 460")) {
        std::cerr << "ImGui OpenGL backend init failed." << std::endl;
        return false;
    }
    
    return true;
}

void UIManager::render(state::AppState& state, GLFWwindow* window) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    bool fontsChanged = g_settingsManager.apply(state.settings);
    if (fontsChanged) {
        ImGui_ImplOpenGL3_DestroyFontsTexture();
        ImGui_ImplOpenGL3_CreateFontsTexture();
    }

    ImGui::NewFrame();

    renderContextMenu(state);

    renderMainMenu(state);

    if (state.settings.flags.openSettings) {
        ImGui::OpenPopup("settings_popup");
        state.settings.flags.openSettings = false;
        state.settings.flags.settingsVisible = true;
    }

    renderSettingsPopup(state);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::renderContextMenu(state::AppState& state) {
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("context_menu");
    }

    if (ImGui::BeginPopup("context_menu")) {
        if (ImGui::MenuItem("Cube"))          { 
            state.renderState.menuCase = 0;
            state.renderState.distance = 0.0f;
        }
        if (ImGui::MenuItem("Trimmed Cubes")) { 
            state.renderState.menuCase = 1;
            state.renderState.distance = 0.0f;
        }
        if (ImGui::MenuItem("Cubes"))         { 
            state.renderState.menuCase = 2;
            state.renderState.distance = 0.0f;
        }
        if (ImGui::MenuItem("Spheres")) {
            state.renderState.menuCase = 3;
            state.renderState.distance = 0.0f;
        }
        if (ImGui::MenuItem("Wormhole")) {
            state.renderState.menuCase = 4;
            state.renderState.distance = 4.0f;
        }
        if (ImGui::MenuItem("Mandelbulb")) {
            state.renderState.menuCase = 5;
            state.renderState.distance = -1.0f;
        }
        if (ImGui::MenuItem("Exit")) {
            exit(EXIT_SUCCESS);
        }
        ImGui::EndPopup();
    }
}

void UIManager::renderMainMenu(state::AppState& state) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(22.0f, 20.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(14.0f, 10.0f));
    ImGui::SetNextWindowSize(ImVec2(420, 0.0f), ImGuiCond_Once);
    ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::SetWindowPos(ImVec2(0, 0));

    const ImVec4 accent = ImVec4(0.43f, 0.70f, 1.0f, 1.0f);
    const ImVec4 subtle = ImVec4(0.60f, 0.65f, 0.76f, 1.0f);

    const ImGuiTableFlags tableFlags =
        ImGuiTableFlags_SizingStretchProp |
        ImGuiTableFlags_BordersInnerV |
        ImGuiTableFlags_NoSavedSettings;

    if (ImGui::BeginTable("scene_sliders", 2, tableFlags)) {
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 110.0f);
        ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextColored(subtle, "Render Steps");
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderInt("##render_steps", &state.renderState.renderSteps,
                         g_sliderConfig.renderSteps.min, g_sliderConfig.renderSteps.max);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextColored(subtle, "Camera Distance");
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderFloat("##camera_distance", &state.renderState.distance,
                           g_sliderConfig.distance.min, g_sliderConfig.distance.max);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextColored(subtle, "Time Speed");
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderInt("##time_speed", &state.renderState.timeSpeed,
                         g_sliderConfig.timeSpeed.min, g_sliderConfig.timeSpeed.max);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextColored(subtle, "Time Offset");
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderFloat("##time_offset", &state.renderState.t,
                           g_sliderConfig.time.min, g_sliderConfig.time.max);

        ImGui::EndTable();
    }

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::TextColored(subtle, "Mouse offset: (%.1f, %.1f)", state.inputState.dx, state.inputState.dy);
    ImGui::SameLine();
    ImGui::TextColored(subtle, "FPS: %.1f", ImGui::GetIO().Framerate);

    const float settingsButtonWidth = 120.0f;
    ImVec2 nextLineStart = ImGui::GetCursorPos();
    ImVec2 buttonPos = ImVec2(
        ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x - settingsButtonWidth,
        ImGui::GetCursorScreenPos().y - ImGui::GetTextLineHeight());
    ImGui::SetCursorScreenPos(buttonPos);
    if (ImGui::Button("Settings", ImVec2(settingsButtonWidth, 0.0f))) {
        state.settings.flags.openSettings = true;
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Open settings (Ctrl + S)");
    }
    ImGui::SetCursorPos(nextLineStart);

    const bool autoReload = state.settings.interaction.shaderAutoReload;
    ImGui::Spacing();
    ImGui::TextColored(autoReload ? accent : subtle,
                       "Auto reload: %s", autoReload ? "enabled" : "disabled");

    ImGui::End();
    ImGui::PopStyleVar(2);
}

void UIManager::renderSettingsPopup(state::AppState &state) {
  bool open = ImGui::BeginPopupModal("settings_popup", nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize);
  if (open) {
    state.settings.flags.settingsVisible = true;

    static const float kUiScaleOptions[] = {0.5f, 0.75f, 1.0f, 1.25f,
                                            1.5f, 2.0f,  3.0f};
    static const char *kUiScaleLabels[] = {"50%",  "75%",  "100%", "125%",
                                           "150%", "200%", "300%"};
    constexpr int kUiScaleCount =
        static_cast<int>(sizeof(kUiScaleOptions) / sizeof(float));

    float currentScale = state.settings.visual.uiScale;
    int currentIndex = 0;
    float bestDiff = std::numeric_limits<float>::max();
    for (int i = 0; i < kUiScaleCount; ++i) {
      float diff = std::fabs(currentScale - kUiScaleOptions[i]);
      if (diff < bestDiff) {
        bestDiff = diff;
        currentIndex = i;
      }
    }

    bool snapped = false;
    if (bestDiff > 1e-4f) {
      state.settings.visual.uiScale = kUiScaleOptions[currentIndex];
      currentScale = state.settings.visual.uiScale;
      snapped = true;
    }

    int selection = currentIndex;
    if (ImGui::Combo("UI Scale", &selection, kUiScaleLabels, kUiScaleCount)) {
      state.settings.visual.uiScale = kUiScaleOptions[selection];
      state.settings.flags.dirty = true;
    } else if (snapped) {
      state.settings.flags.dirty = true;
    }
    ImGui::SameLine();
    ImGui::Text("%.2fx", state.settings.visual.uiScale);

    bool &autoReload = state.settings.interaction.shaderAutoReload;
    if (ImGui::Checkbox("Enable shader auto reload", &autoReload)) {
      state.settings.flags.dirty = true;
    }

    ImGui::Spacing();
    if (ImGui::Button("Close")) {
      ImGui::CloseCurrentPopup();
      state.settings.flags.settingsVisible = false;
      state.settings.flags.closeSettings = false;
    }

    if (state.settings.flags.closeSettings) {
      state.settings.flags.closeSettings = false;
      state.settings.flags.settingsVisible = false;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  } else {
    state.settings.flags.settingsVisible = false;
    state.settings.flags.closeSettings = false;
  }
}

} // namespace ui 
