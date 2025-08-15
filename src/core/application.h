#pragma once

#include "../config/config_manager.h"
#include "../state/app_state.h"

#include <GLFW/glfw3.h>

namespace core {

/**
 * Основной класс приложения
 */
class Application {
public:
    /**
     * Конструктор
     */
    Application();
    
    /**
     * Деструктор
     */
    ~Application();
    
    /**
     * Инициализирует приложение
     * @return true если инициализация успешна
     */
    bool init();
    
    /**
     * Запускает главный цикл приложения
     */
    void run();
    
    /**
     * Завершает работу приложения
     */
    void shutdown();

private:
    /**
     * Обрабатывает события
     */
    void processEvents();
    
    /**
     * Обновляет состояние приложения
     */
    void update();
    
    /**
     * Рендерит кадр
     */
    void render();
    
    /**
     * Проверяет изменения конфигурации
     */
    void checkConfigChanges();

private:
    struct {
        GLFWwindow *window = nullptr;
        state::AppState state;
        config::ConfigManager *configManager = nullptr;
        bool initialized = false;
    } data;
};

} // namespace core 