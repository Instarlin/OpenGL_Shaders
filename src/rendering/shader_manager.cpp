#include "shader_manager.h"
#include "../utils/file_utils.h"
#include "../utils/error_utils.h"
#include <iostream>

namespace rendering {

GLuint ShaderManager::createShader(const char* glslCode, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &glslCode, nullptr);
    glCompileShader(shader);
    
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        printShaderLog(shader);
    }
    
    return shader;
}

GLuint ShaderManager::createShaderProgram(const std::string& vertexPath, 
                                        const std::string& fragmentPath) {
    std::string vertexShaderStr = utils::readFile(vertexPath);
    std::string fragmentShaderStr = utils::readFile(fragmentPath);
    const char* vertexShader = vertexShaderStr.c_str();
    const char* fragmentShader = fragmentShaderStr.c_str();

    std::cout << "Loading vertex shader from: " << vertexPath << std::endl;
    std::cout << "Loading fragment shader from: " << fragmentPath << std::endl;

    GLuint vShader = createShader(vertexShader, GL_VERTEX_SHADER);
    GLuint fShader = createShader(fragmentShader, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    GLint result;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (result != GL_TRUE) {
        printProgramLog(shaderProgram);
    }

    return shaderProgram;
}

GLuint ShaderManager::recreateShaderProgram(state::AppState& state, int resetShader) {
    if (resetShader == 1) {
        glDeleteProgram(state.renderState.renderingProgram);
    }

    return createShaderProgram(state.renderState.vertexPath, state.renderState.fragmentPath);
}

void ShaderManager::initializeUniforms(state::AppState& state) {
    state.renderState.uniforms.init(state.renderState.renderingProgram);
}

void ShaderManager::printShaderLog(GLuint shader) {
    utils::printShaderLog(shader);
}

void ShaderManager::printProgramLog(GLuint program) {
    utils::printProgramLog(program);
}

} // namespace rendering 