#include <cstdint>
#include <filesystem>

#include <stdio.h>
#include <string>
#include <vector>

#include "../libs/imgui/backends/imgui_impl_glfw.h"
#include "../libs/imgui/backends/imgui_impl_opengl3.h"
#include "../libs/imgui/imgui.h"
#include <GLFW/glfw3.h>

#include "../libs/stb/stb_image.h"
#include "../libs/stb/stb_image_resize2.h"
#include "../libs/stb/stb_image_write.h"

#include "../include/converter_state.h"
#include "../include/imgui_utils.h"

// Forward declarations
void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
void DropCallback(GLFWwindow *window, int count, const char **paths);
bool LoadTextureFromFile(const char *filename, GLuint *out_texture,
                         int *out_width, int *out_height, int *out_channels);
void ProcessConvert(AppState &state);
void UpdatePreview(AppState &state);

// Global state
static AppState g_State;

int main(int, char **) {
  // Setup window
  glfwSetErrorCallback([](int error, const char *description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
  });
  if (!glfwInit())
    return 1;

  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  GLFWwindow *window =
      glfwCreateWindow(1280, 720, "Neon TGA Converter", NULL, NULL);
  if (window == NULL)
    return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
  glfwSetDropCallback(window, DropCallback);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ApplyNeonStyle();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
      ImGui::SetNextWindowPos(ImVec2(0, 0));
      ImGui::SetNextWindowSize(io.DisplaySize);
      ImGui::Begin("Neon TGA Converter", NULL,
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                       ImGuiWindowFlags_NoCollapse |
                       ImGuiWindowFlags_NoTitleBar |
                       ImGuiWindowFlags_NoBringToFrontOnFocus);

      ImGui::PushFont(io.Fonts->Fonts[0]);
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.7f, 1.0f), "NEON TGA CONVERTER");
      ImGui::PopFont();
      ImGui::Separator();
      ImGui::Spacing();

      // 1. Files List
      ImGui::Text("1. Files (%d)", (int)g_State.pendingFiles.size());
      ImGui::SameLine();
      HelpMarker("Drag and drop multiple files to add them.");

      if (ImGui::Button("Clear All")) {
        g_State.pendingFiles.clear();
        g_State.selectedFileIndex = -1;
        g_State.textureID = 0;
      }

      ImGui::BeginChild("FilesList", ImVec2(0, 150), true);
      for (int i = 0; i < g_State.pendingFiles.size(); i++) {
        std::string fileName =
            std::filesystem::path(g_State.pendingFiles[i]).filename().string();
        if (ImGui::Selectable(fileName.c_str(),
                              g_State.selectedFileIndex == i)) {
          g_State.selectedFileIndex = i;
          UpdatePreview(g_State);
        }
      }
      ImGui::EndChild();

      ImGui::Spacing();

      // 2. Preview & Info
      if (g_State.selectedFileIndex >= 0 &&
          g_State.selectedFileIndex < g_State.pendingFiles.size()) {
        ImGui::Separator();
        ImGui::Text("Creating TGA from: %s",
                    g_State.pendingFiles[g_State.selectedFileIndex].c_str());
        ImGui::Text("Original: %dx%d (%d channels)", g_State.width,
                    g_State.height, g_State.channels);

        if (g_State.textureID != 0) {
          float aspect = (float)g_State.height / (float)g_State.width;
          float previewWidth = 200.0f;
          float previewHeight = previewWidth * aspect;
          ImGui::Image((void *)(intptr_t)g_State.textureID,
                       ImVec2(previewWidth, previewHeight));
        }
      } else {
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
                           "Select a file from the list to preview.");
      }

      ImGui::Spacing();
      ImGui::Separator();
      ImGui::Spacing();

      // 3. Settings
      ImGui::Text("2. Global Settings");
      bool sizeChanged = false;
      sizeChanged |= ImGui::InputInt("Target Width", &g_State.targetWidth);
      sizeChanged |= ImGui::InputInt("Target Height", &g_State.targetHeight);
      ImGui::Checkbox("Keep Aspect Ratio", &g_State.keepAspectRatio);

      if (g_State.keepAspectRatio && sizeChanged && g_State.width > 0 &&
          g_State.height > 0) {
        if (g_State.targetWidth > 0) {
          g_State.targetHeight =
              (int)(g_State.targetWidth *
                    ((float)g_State.height / (float)g_State.width));
        }
      }
      ImGui::Spacing();
      ImGui::Separator();
      ImGui::Spacing();

      // 4. Action
      if (ImGui::Button("CONVERT ALL", ImVec2(200, 50))) {
        if (!g_State.pendingFiles.empty()) {
          ProcessConvert(g_State);
        } else {
          g_State.statusMessage = "No files to convert!";
          g_State.showError = true;
        }
      }

      ImGui::Spacing();
      if (g_State.showSuccess) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s",
                           g_State.statusMessage.c_str());
      } else if (g_State.showError) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s",
                           g_State.statusMessage.c_str());
      } else {
        ImGui::Text("%s", g_State.statusMessage.c_str());
      }

      ImGui::End();
    }

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.05f, 0.05f, 0.08f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

void FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void UpdatePreview(AppState &state) {
  if (state.selectedFileIndex < 0 ||
      state.selectedFileIndex >= state.pendingFiles.size())
    return;

  if (state.textureID != 0) {
    glDeleteTextures(1, &state.textureID);
    state.textureID = 0;
  }

  std::string path = state.pendingFiles[state.selectedFileIndex];
  if (LoadTextureFromFile(path.c_str(), &state.textureID, &state.width,
                          &state.height, &state.channels)) {
    if (state.targetWidth == 0)
      state.targetWidth = state.width;
    if (state.targetHeight == 0)
      state.targetHeight = state.height;
  }
}

void DropCallback(GLFWwindow *window, int count, const char **paths) {
  for (int i = 0; i < count; i++) {
    g_State.pendingFiles.push_back(paths[i]);
  }

  if (count > 0 && g_State.selectedFileIndex == -1) {
    g_State.selectedFileIndex = g_State.pendingFiles.size() - count;
    UpdatePreview(g_State);
  }
  g_State.statusMessage = "Added " + std::to_string(count) + " files.";
  g_State.showSuccess = false;
  g_State.showError = false;
}

bool LoadTextureFromFile(const char *filename, GLuint *out_texture,
                         int *out_width, int *out_height, int *out_channels) {
  int image_width = 0;
  int image_height = 0;
  int image_channels = 0;
  unsigned char *image_data =
      stbi_load(filename, &image_width, &image_height, &image_channels, 4);
  if (image_data == NULL)
    return false;

  GLuint image_texture;
  glGenTextures(1, &image_texture);
  glBindTexture(GL_TEXTURE_2D, image_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image_data);
  stbi_image_free(image_data);

  *out_texture = image_texture;
  *out_width = image_width;
  *out_height = image_height;
  *out_channels = image_channels;

  return true;
}

void ProcessConvert(AppState &state) {
  int successCount = 0;
  int failCount = 0;

  for (const auto &file : state.pendingFiles) {
    int width, height, channels;
    unsigned char *imgData =
        stbi_load(file.c_str(), &width, &height, &channels, 0);
    if (!imgData) {
      failCount++;
      continue;
    }

    unsigned char *dataToWrite = imgData;
    std::vector<unsigned char> resizedData;
    int writeWidth = width;
    int writeHeight = height;

    int finalTargetW = state.targetWidth > 0 ? state.targetWidth : width;
    int finalTargetH = state.targetHeight > 0 ? state.targetHeight : height;

    // Recalculate height if Aspect Ratio is kept AND user provided a width
    if (state.keepAspectRatio && state.targetWidth > 0) {
      float ratio = (float)height / (float)width;
      finalTargetH = (int)(state.targetWidth * ratio);
    }

    if (finalTargetW != width || finalTargetH != height) {
      resizedData.resize(finalTargetW * finalTargetH * channels);
      stbir_resize_uint8_linear(imgData, width, height, 0, resizedData.data(),
                                finalTargetW, finalTargetH, 0,
                                (stbir_pixel_layout)channels);
      dataToWrite = resizedData.data();
      writeWidth = finalTargetW;
      writeHeight = finalTargetH;
    }

    std::filesystem::path p(file);
    std::string outputPath =
        p.parent_path().string() + "/" + p.stem().string() + "_converted.tga";

    int result = stbi_write_tga(outputPath.c_str(), writeWidth, writeHeight,
                                channels, dataToWrite);
    if (result)
      successCount++;
    else
      failCount++;

    stbi_image_free(imgData);
  }

  state.statusMessage = "Converted: " + std::to_string(successCount) +
                        " | Failed: " + std::to_string(failCount);
  state.showSuccess = (failCount == 0);
  state.showError = (failCount > 0);
}
