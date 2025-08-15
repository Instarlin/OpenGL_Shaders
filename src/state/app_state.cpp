#include "app_state.h"

namespace state {

void Uniforms::init(GLuint program) {
    offset = glGetUniformLocation(program, "offset");
    time = glGetUniformLocation(program, "time");
    width = glGetUniformLocation(program, "width");
    height = glGetUniformLocation(program, "height");
    menuCase = glGetUniformLocation(program, "menuCase");
    posOffset = glGetUniformLocation(program, "posOffset");
    rotateSpeed = glGetUniformLocation(program, "rotateSpeed");
    renderSteps = glGetUniformLocation(program, "renderSteps");
    mouseX = glGetUniformLocation(program, "mouseX");
    mouseY = glGetUniformLocation(program, "mouseY");
}

void AppState::reset() {
    renderState = RenderState{};
    inputState = InputState{};
}

} // namespace state 