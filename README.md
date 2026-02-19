# Neon TGA Converter

A simple, neon-styled GUI application to convert images to TGA format. Built with C++, ImGui, and OpenGL.

## Features
- **Drag & Drop** support for multiple files.
- **Preview** selected images before conversion.
- **Resize** options (Width/Height) with Aspect Ratio locking.
- **Neon UI** theme.
- **Fast Conversion** using stb_image and stb_image_resize.

## Build

### Prerequisites
- CMake 3.20+
- C++20 Compiler
- GLFW3 (install via package manager or vcpkg)

### Windows
```powershell
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## License
MIT License. See [LICENSE](LICENSE) for details.
