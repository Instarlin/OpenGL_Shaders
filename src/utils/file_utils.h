#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace utils {

/**
 * Reads the contents of a file into a string
 * @param filepath Path to the file
 * @return The contents of the file as a string
 * @throws std::runtime_error if the file cannot be opened
 */
std::string readFile(const std::string& filepath);

/**
 * Checks if a file exists
 * @param filepath Path to the file
 * @return true if the file exists, false otherwise
 */
bool fileExists(const std::string& filepath);

/**
 * Gets the file extension
 * @param filepath Path to the file
 * @return The file extension (without the dot)
 */
std::string getFileExtension(const std::string& filepath);

/**
 * Gets the file name without the path
 * @param filepath Full path to the file
 * @return The file name
 */
std::string getFileName(const std::string& filepath);

} // namespace utils 