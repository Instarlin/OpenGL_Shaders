#pragma once

#include <string>

namespace utils {

/**
 * Проверяет ошибки OpenGL и выводит их в консоль
 * @param context Контекст для сообщения об ошибке
 * @return true если есть ошибка, false иначе
 */
bool checkOpenGLError(const std::string& context = "");

/**
 * Выводит информацию о версии OpenGL
 */
void printOpenGLVersionInfo();

/**
 * Выводит лог программы OpenGL
 * @param program ID программы OpenGL
 */
void printProgramLog(unsigned int program);

/**
 * Выводит лог шейдера OpenGL
 * @param shader ID шейдера OpenGL
 */
void printShaderLog(unsigned int shader);

} // namespace utils 