# 3D Object Renderer

A lightweight rendering application built in C++ using modern graphics libraries and an **Entity Component System (ECS)** architecture. Currently designed to run on **Windows** systems.

This project is licensed under the MIT License, see [LICENSE](/LICENSE).

---

## Features

- **Entity Component System (ECS)** using [EnTT](https://github.com/skypjack/entt)
- **Real-time rendering** with OpenGL via GLFW
- **GUI integration** using [ImGui](https://github.com/ocornut/imgui)
- Interactive viewport for rendering scenes with multiple objects
- Camera control through standard movement
- Loading of objects from .obj files to mesh classes that are usable at runtime

## Dependencies

- [GLFW](https://www.glfw.org/) – Window and input management
- [Glad](https://github.com/Dav1dde/glad) – OpenGL loader
- [GLM](https://github.com/g-truc/glm) – Mathematics library
- [EnTT](https://github.com/skypjack/entt) – Entity Component System
- [ImGui](https://github.com/ocornut/imgui) – GUI library

> All dependencies are included either in the CMake project or the deps folder. No manual linking is required.

---

## Building the Project

The project uses **CMake** for an easy build process:

1. **Clone the repository**:

```bash
git clone https://github.com/AngusWebbHammond/Object-Renderer-GLFW.git
cd Object-Renderer-GLFW
```

2. **Create a build directory**:

```bash
mkdir build
cd build
```

3. **Run CMake** to configure the project:

```bash
cmake ..
```

4. **Build the project**:

```bash
cmake --build .
```

5. **Run the executable**:

```bash
cd Debug
.\main.exe
```
