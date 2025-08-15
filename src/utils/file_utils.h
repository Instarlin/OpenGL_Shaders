#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace utils {

/**
 * Читает содержимое файла в строку
 * @param filepath Путь к файлу
 * @return Содержимое файла как строка
 * @throws std::runtime_error если файл не может быть открыт
 */
std::string readFile(const std::string& filepath);

/**
 * Проверяет существование файла
 * @param filepath Путь к файлу
 * @return true если файл существует, false иначе
 */
bool fileExists(const std::string& filepath);

/**
 * Получает расширение файла
 * @param filepath Путь к файлу
 * @return Расширение файла (без точки)
 */
std::string getFileExtension(const std::string& filepath);

/**
 * Получает имя файла без пути
 * @param filepath Полный путь к файлу
 * @return Имя файла
 */
std::string getFileName(const std::string& filepath);

} // namespace utils 