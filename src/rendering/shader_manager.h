#pragma once

#include <string>
#include <GL/glew.h>
#include "../state/app_state.h"

namespace rendering {

/**
 * Shader manager class
 */
class ShaderManager {
public:
    /**
     * Creates a shader from source code
     * @param glslCode Shader source code
     * @param type Shader type (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER)
     * @return ID of the created shader
     */
    static GLuint createShader(const char* glslCode, GLenum type);
    
    /**
     * Creates a shader program from files
     * @param vertexPath Path to vertex shader
     * @param fragmentPath Path to fragment shader
     * @return ID of the created program
     */
    static GLuint createShaderProgram(const std::string& vertexPath, 
                                    const std::string& fragmentPath);
    
    /**
     * Recreates the shader program
     * @param state Application state
     * @param resetShader Flag to recreate
     * @return ID of the new program
     */
    static GLuint recreateShaderProgram(state::AppState& state, int resetShader = 0);
    
    /**
     * Initializes uniform variables
     * @param state Application state
     */
    static void initializeUniforms(state::AppState& state);

private:
    static void printShaderLog(GLuint shader);
    static void printProgramLog(GLuint program);
};

} // namespace rendering 