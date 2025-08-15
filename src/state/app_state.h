#pragma once

#include <string>
#include <GL/glew.h>
// #include "../config/slider_config.h"

namespace state {

/**
 * Uniform variables
 */
struct Uniforms {
    GLuint offset;
    GLuint time;
    GLuint width;
    GLuint height;
    GLuint menuCase;
    GLuint posOffset;
    GLuint rotateSpeed;
    GLuint renderSteps;
    GLuint mouseX;
    GLuint mouseY;

    void init(GLuint program);
};

/**
 * Input state
 */
struct InputState {
    bool isLeftCtrlDown = false;
    bool isLeftMouseDown = false;
    
    double mouseX = 0.0;
    double mouseY = 0.0;
    double dx;
    double dy;
    double offsetX = 0.0;
    double offsetY = 0.0;
    double totalX;
    double totalY;

    float scrollSensitivity;
};

/**
 * Default values for render state
 */
struct RenderState {
    int width;
    int height;
    
    std::string vertexPath;
    std::string fragmentPath;
    
    GLuint renderingProgram = 0;
    GLuint vShader = 0;
    GLuint fShader = 0;
    GLuint vbo[1];
    
    int menuCase = 0;
    float distance = 0.0f;
    int renderSteps = 80;
    int timeSpeed = 10;
    
    float t = 0.0f;
    float inc = 0.01f;
    
    Uniforms uniforms;
};

/**
 * Main application state
 */
struct AppState {
    RenderState renderState;
    InputState inputState;
    
    void reset();
};

} // namespace state 