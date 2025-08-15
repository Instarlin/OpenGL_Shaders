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
- **Sliders**: Adjust rendering steps, distance, rotation speed, time, etc.
- **Mouse Drag (Middle Button)**: Pan the scene.
- **Scroll Wheel**: Adjust default value parameter.
- **Keyboard**:
  - `Left Ctrl + R`: Reload shaders
  - `Left Ctrl + 1/2/3/4`: Switch between scene types

### Building

1. **Clone the repository** and ensure all submodules (ImGui, etc.) are present in `include/`.
2. **Execute** the build script:

   ```
   ./build.sh
   ```
3. **That's should be it**

### Configuration

- **Config**: Change config in /build/Release/config.json
- **Shaders**: Add or modify GLSL files in the `/build/Release/glsl/` directory.
- **Icons**: Replace or add icons in the `/build/Release/ico/` directory.

### Notes

- The project automatically creates `config.json` on first run
- Configuration changes are detected and applied in real-time

### Dependencies

- [CMake](https://cmake.org/) >= 3.28
- [GLFW](https://www.glfw.org/)
- [GLEW](http://glew.sourceforge.net/)
- [ImGui](https://github.com/ocornut/imgui)
- [nlohmann/json](https://github.com/nlohmann/json) - JSON parsing
- OpenGL 4.6