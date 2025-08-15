#include "renderer.h"
#include "shader_manager.h"
#include "../utils/error_utils.h"
#include <iostream>

namespace rendering {

void Renderer::init(state::AppState& state, GLFWwindow* window) {
    glfwMakeContextCurrent(window);
    if (glfwGetCurrentContext() == nullptr) {
        std::cerr << "GLFW context not current." << std::endl;
        exit(EXIT_FAILURE);
    }

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init failed." << std::endl;
        exit(EXIT_FAILURE);
    }
    utils::printOpenGLVersionInfo();

    state.renderState.renderingProgram = ShaderManager::createShaderProgram(
        state.renderState.vertexPath, state.renderState.fragmentPath);
    ShaderManager::initializeUniforms(state);

    setupVertexArray(state);

    glfwSwapInterval(1);
}

void Renderer::render(state::AppState& state, GLFWwindow* window, double currentTime) {
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(state.renderState.renderingProgram);

    updateRenderState(state);
    updateUniforms(state);

    utils::checkOpenGLError("Render");

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::updateUniforms(state::AppState& state) {
    auto& uniforms = state.renderState.uniforms;
    auto& renderState = state.renderState;
    auto& inputState = state.inputState;

    // Uniforms mess 
    // glProgramUniform1f(renderState.renderingProgram, uniforms.offset, renderState.x);
    glProgramUniform1f(renderState.renderingProgram, uniforms.time, renderState.t);
    glProgramUniform1i(renderState.renderingProgram, uniforms.width, renderState.width);
    glProgramUniform1i(renderState.renderingProgram, uniforms.height, renderState.height);
    glProgramUniform1i(renderState.renderingProgram, uniforms.menuCase, renderState.menuCase);
    glProgramUniform1f(renderState.renderingProgram, uniforms.posOffset, renderState.distance);
    glProgramUniform1i(renderState.renderingProgram, uniforms.renderSteps, renderState.renderSteps);
    glProgramUniform1f(renderState.renderingProgram, uniforms.mouseX, inputState.totalX);
    glProgramUniform1f(renderState.renderingProgram, uniforms.mouseY, inputState.totalY);
}

void Renderer::shutdown(state::AppState &state) {
    if (state.renderState.renderingProgram != 0) {
        glDeleteProgram(state.renderState.renderingProgram);
    }
}

void Renderer::setupVertexArray(state::AppState& state) {
    glGenVertexArrays(1, state.renderState.vbo);
    glBindVertexArray(state.renderState.vbo[0]);
}

void Renderer::updateRenderState(state::AppState& state) {
    state.renderState.t += state.renderState.inc * state.renderState.timeSpeed * 0.1f;
}

} // namespace rendering 