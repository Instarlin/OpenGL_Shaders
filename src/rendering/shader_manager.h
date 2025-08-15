#pragma once

#include <string>
#include <GL/glew.h>
#include "../state/app_state.h"

namespace rendering {

/**
 * Менеджер шейдеров
 */
class ShaderManager {
public:
    /**
     * Создает шейдер из исходного кода
     * @param glslCode Исходный код шейдера
     * @param type Тип шейдера (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER)
     * @return ID созданного шейдера
     */
    static GLuint createShader(const char* glslCode, GLenum type);
    
    /**
     * Создает программу шейдеров из файлов
     * @param vertexPath Путь к vertex шейдеру
     * @param fragmentPath Путь к fragment шейдеру
     * @return ID созданной программы
     */
    static GLuint createShaderProgram(const std::string& vertexPath, 
                                    const std::string& fragmentPath);
    
    /**
     * Пересоздает программу шейдеров
     * @param state Состояние приложения
     * @param resetShader Флаг для пересоздания
     * @return ID новой программы
     */
    static GLuint recreateShaderProgram(state::AppState& state, int resetShader = 0);
    
    /**
     * Инициализирует uniform переменные
     * @param state Состояние приложения
     */
    static void initializeUniforms(state::AppState& state);

private:
    static void printShaderLog(GLuint shader);
    static void printProgramLog(GLuint program);
};

} // namespace rendering 