cmake_minimum_required(VERSION 3.20)
project(NeonTGAConverter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Definitions
# add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GL3W) # Commented out, user doesn't seem to have gl3w

# Include directories
include_directories(include)
include_directories(libs)
include_directories(libs/imgui)
include_directories(libs/stb)

# Find Packages
include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

# Sources
set(SOURCES
  src/main.cpp
  src/imgui_utils.cpp
  src/stb_impl.cpp
  libs/imgui/imgui.cpp
  # libs/imgui/imgui_demo.cpp # Missing in user repo
  libs/imgui/imgui_draw.cpp
  libs/imgui/imgui_tables.cpp
  libs/imgui/imgui_widgets.cpp
  libs/imgui/backends/imgui_impl_glfw.cpp
  libs/imgui/backends/imgui_impl_opengl3.cpp
)

# Executable
add_executable(NeonTGAConverter ${SOURCES})

# Linking
target_link_libraries(NeonTGAConverter PRIVATE glfw OpenGL::GL)

# Copy assets if any (none for now, but good to have ready)
