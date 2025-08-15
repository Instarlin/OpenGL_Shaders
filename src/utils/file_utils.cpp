#include "file_utils.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace utils {

std::string readFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return std::string(buffer.begin(), buffer.end());
}

bool fileExists(const std::string& filepath) {
    return std::filesystem::exists(filepath);
}

std::string getFileExtension(const std::string& filepath) {
    std::filesystem::path path(filepath);
    return path.extension().string().substr(1); // Убираем точку
}

std::string getFileName(const std::string& filepath) {
    std::filesystem::path path(filepath);
    return path.filename().string();
}

} // namespace utils 