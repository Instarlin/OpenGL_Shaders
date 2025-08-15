#include "ui_manager.h"
#include "../backends/imgui_impl_glfw.h"
#include "../backends/imgui_impl_opengl3.h"
#include "../config/slider_config.h"
#include <imgui.h>
#include <iostream>

namespace ui {

bool UIManager::init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    // ImGui::GetStyle().WindowRounding = 10.0f;
    // ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 1.0f;

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
    ImGui::NewFrame();

    renderContextMenu(state);

    renderMainMenu(state);

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
    ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::SetWindowSize(ImVec2(400, 170));
    ImGui::SetWindowPos(ImVec2(0, 0));

    ImGui::SliderInt("RenderSteps", &state.renderState.renderSteps,
                     config.renderSteps.min, config.renderSteps.max);
    ImGui::SliderFloat("Distance", &state.renderState.distance,
                       config.distance.min, config.distance.max);
    ImGui::SliderInt("Time speed", &state.renderState.timeSpeed,
                     config.timeSpeed.min, config.timeSpeed.max);
    ImGui::SliderFloat("Time", &state.renderState.t, config.time.min,
                       config.time.max);

    ImGui::Text("Mouse offset: (%.1f, %.1f)", state.inputState.dx,
                state.inputState.dy);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();
}

} // namespace ui 