#include "error_utils.h"
#include <GL/glew.h>
#include <iostream>
#include <vector>

namespace utils {

bool checkOpenGLError(const std::string& context) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL Error" << (context.empty() ? "" : " in " + context) 
                  << ": " << error << std::endl;
        return true;
    }
    return false;
}

void printOpenGLVersionInfo() {
    std::cout << "Vendor:   " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version:  " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void printProgramLog(unsigned int program) {
    int len = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        std::vector<char> log(len);
        glGetProgramInfoLog(program, len, nullptr, log.data());
        std::cout << "<Program Info Log> " << log.data() << std::endl;
    }
}

void printShaderLog(unsigned int shader) {
    int len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        std::vector<char> log(len);
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        std::cout << "<Shader Info Log> " << log.data() << std::endl;
    }
}

} // namespace utils 