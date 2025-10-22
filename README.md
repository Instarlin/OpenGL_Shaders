![Cube](https://github.com/Instarlin/Instarlin/blob/main/readme_assets/cube.gif)
![Trimmed Cubes](https://github.com/Instarlin/Instarlin/blob/main/readme_assets/trimmed.gif)

# 3D Scene Renderer with GLSL Shaders

## Overview

This project is a real-time 3D scene renderer written in C++ using OpenGL, GLFW, GLEW, and ImGui.

### Shaders

- `glsl/vertex.glsl` - Main vertex shader
- `glsl/fragment.glsl` - Main fragment shader
- `glsl/noiseCloud.glsl`, `glsl/noise2D.glsl`, `glsl/noise3D.glsl` - Procedural noise shaders

### Controls

- **Right Mouse Button**: Open ImGui context menu for scene selection and exit.
- **Sliders**: Adjust rendering steps, camera distance, animation speed, and time offset.
- **Mouse Drag (Middle Button)**: Pan the scene.
- **Scroll Wheel**: Adjust camera distance (scaled by sensitivity from the config).
- **Keyboard**:
  - `Left Ctrl + R`: Reload shaders and re-upload uniforms.
  - `Left Ctrl + 1/2/3/4`: Switch between scene presets.
  - `Left Ctrl + S`: Toggle the Settings popup.
- **Settings Popup**: Adjust UI scale and toggle shader auto-reload. The popup can be opened from the “Settings” button or via `Ctrl + S`.

### Building

1. **Clone the repository** and ensure all submodules (ImGui, etc.) are present in `include/`.
2. **Inspect available build options** (optional):

   ```
   ./build.sh -h
   ```

   Useful flags:

   - `-s` – allow Conan to install missing system packages (may prompt for sudo).
   - `-l` – launch the executable automatically after a successful build.
   - `BUILD_TYPE=Debug ./build.sh` – override the default `Release` build type.

3. **Execute** the build script:

   ```
   ./build.sh
   ```
4. **Launch** the app manually (if you did not pass `-l`) from `build/<type>/3DShader`.

### Configuration

- **Config**: `config.json` is generated next to the executable on first launch (e.g., `build/Release/config.json`). Edit it to adjust window size, shader paths, and input sensitivity, then restart the application to apply changes.
- **Settings**: Runtime preferences (UI scale, shader auto-reload) are stored in `settings.json`. Changes made in the Settings popup are saved automatically on exit or when toggled.
- **Shaders**: Add or modify GLSL files in the `/build/Release/glsl/` directory.
- **Icons**: Replace or add icons in the `/build/Release/ico/` directory.

### Notes

- The project automatically creates `config.json` and `settings.json` on first run.
- Shader auto-reload watches the active vertex and fragment shader files when enabled.
- Manual shader reload (`Ctrl + R`) is always available.

### Dependencies

- [CMake](https://cmake.org/) >= 3.28
- [GLFW](https://www.glfw.org/)
- [GLEW](http://glew.sourceforge.net/)
- [ImGui](https://github.com/ocornut/imgui)
- [nlohmann/json](https://github.com/nlohmann/json) - JSON parsing
- OpenGL 4.6
