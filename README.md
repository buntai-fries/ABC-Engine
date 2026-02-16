## ABC-Engine
-> ABC-Engine is a lightweight, cross-platform C++ graphics engine built for 3D graphics rendering mainly for learning purpose.
It leverages modern OpenGL for core rendering, GLFW for window/input management, and ImGui for an immediate-mode UI debug layer.
Designed as a learning project for graphics programming enthusiasts, 
it supports basic scene rendering, camera controls, and extensible architecture for features like ray tracing.

## Features:
```
- Modern OpenGL Rendering**: Vertex/fragment shaders, VAOs/VBOs, textures, and framebuffer support.
- GLFW Integration**: Cross-platform window creation, input handling (keyboard/mouse).
- ImGui Debug UI**: Real-time controls for camera, lighting, and rendering params.
- CMake Build System**: Easy cross-platform builds (Windows/Linux).
```

## Tech Stack
```
| Component | Version/Purpose |
|-----------|-----------------|
| C++       | 17+             |
| OpenGL    | 4.6 Core Profile|
| GLFW      | Window & Input  |
| ImGui     | Debug UI        |
| GLM       | Math Library    |
| CMake     | Build System    |
| stb_image | Texture Loading |
```

## Building the Project
```
1. Clone the repo:
   git clone https://github.com/buntai-fries/ABC-Engine.git
   cd ABC-Engine
   git submodule update --init --recursive

2. Prerequisites (Linux/Fedora recommended):
   - OpenGL/GLFW dev libs: `sudo dnf install glfw-devel mesa-libGL-devel`
   - CMake 3.16+: `sudo dnf install cmake`

3. Build:
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

4. Run:
./ABC-Engine

Troubleshooting:
- Windows: Use vcpkg for deps or MSVC.
- Ensure GLFW/OpenGL drivers are up-to-date.

Usage
- Launch the app to see a spinning cube box scene with lightning.
- Use WASD to move camera, mouse to look around.
```

## Project Structure
```
ABC-Engine/
├── CMakeLists.txt          # Build config
├── src/
│   ├── glad.c             # GLAD
│   ├── imgui/             # ImGui integration
│   ├── Shader.cpp         # Shader Class cpp
│   ├── Shader.hpp         # Shader Class header
│   └── main.cpp           # Entry point
├── assets/                # Textures, GLSL Files
    ├── image/             # Textures
    ├── shader/            # GLSL files
└── README.md              # This file!
```

## Roadmap
```
- [ ] PBR material system
- [ ] Ray tracing (hybrid rasterization/RT)
- [ ] Asset importer (glTF/OBJ)
- [ ] Vulkan backend
- [ ] Editor mode
```
(Note: Might add or remove some of these.)

## License
```
MIT License - see [LICENSE](LICENSE) file. 
© buntai-fries
```

## Acknowledgments
```
Inspired by [LearnOpenGL](https://learnopengl.com) 
Many thanks to GLFW/ImGui teams!
```

