#pragma once

#include <string>

namespace utils {

/**
 * Checks for OpenGL errors and prints them to the console
 * @param context Context for error message
 * @return true if there is an error, false otherwise
 */
bool checkOpenGLError(const std::string& context = "");

/**
 * Prints OpenGL version information
 */
void printOpenGLVersionInfo();

/**
 * Prints OpenGL program log
 * @param program ID of the OpenGL program
 */
void printProgramLog(unsigned int program);

/**
 * Prints OpenGL shader log
 * @param shader ID of the OpenGL shader
 */
void printShaderLog(unsigned int shader);

} // namespace utils 