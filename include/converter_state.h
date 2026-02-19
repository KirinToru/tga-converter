#pragma once
#include <string>
#include <vector>

struct AppState {
  std::vector<std::string> pendingFiles;
  int selectedFileIndex = -1;

  unsigned int textureID = 0;
  int width = 0;
  int height = 0;
  int channels = 0;

  int targetWidth = 0;
  int targetHeight = 0;
  bool keepAspectRatio = true;

  std::string statusMessage = "Ready";
  bool showSuccess = false;
  bool showError = false;
};
