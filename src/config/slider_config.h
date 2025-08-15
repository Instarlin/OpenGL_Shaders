#pragma once

namespace config {

struct SliderConfig {
    struct RenderSteps {
        static constexpr int min = 1;
        static constexpr int max = 1500;
        static constexpr int default_value = 80;
        static constexpr const char *label = "RenderSteps";
    } renderSteps;

    struct TimeSpeed {
        static constexpr int min = 0;
        static constexpr int max = 30;
        static constexpr int default_value = 10;
        static constexpr const char *label = "Time speed";
    } timeSpeed;

    struct Time {
        static constexpr float min = 0.0f;
        static constexpr float max = 262144.0f;
        static constexpr float default_value = 0.0f;
        static constexpr const char *label = "Time";
    } time;

    struct Distance {
        static constexpr float min = -5.0f;
        static constexpr float max = 50.0f;
        static constexpr float default_value = 0.0f;
        static constexpr const char *label = "Distance";
    } distance;
};

/**
 * Метаданные для каждого режима рендеринга
 */
struct RenderModeConfig {
    static constexpr const char* CUBE_NAME = "Cube";
    static constexpr const char* TRIMMED_CUBES_NAME = "Trimmed Cubes";
    static constexpr const char* CUBES_NAME = "Cubes";
    static constexpr const char* SPHERES_NAME = "Spheres";
    
    static constexpr int CUBE_CASE = 0;
    static constexpr int TRIMMED_CUBES_CASE = 1;
    static constexpr int CUBES_CASE = 2;
    static constexpr int SPHERES_CASE = 3;
};

} // namespace config 