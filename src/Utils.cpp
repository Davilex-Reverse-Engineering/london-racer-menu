#include "Utils.hpp"

#include "../vendor/SDL/include/SDL3/SDL.h"

#include <filesystem>

std::string Utils::getFullPath(const std::string &file_name) {
  std::filesystem::path file_path = file_name;
  if (file_path.is_absolute()) {
    return file_path.generic_string();
  }

  std::filesystem::path working_directory; 
  const char * sdl_base_directory = SDL_GetBasePath();
  if (sdl_base_directory) {
    working_directory = std::filesystem::path(sdl_base_directory);
  } else {
    working_directory = std::filesystem::path(".");
  }

  if (std::filesystem::exists(working_directory / file_path)) {
    return (working_directory / file_path).generic_string();
  }

  std::string current_file_name = "";
  std::filesystem::directory_iterator current_directory_files(working_directory);
  for (auto &file : current_directory_files) {
    if (file.is_directory()) {
      continue;
    }
    current_file_name = file.path().filename().generic_string();

    bool files_match = true;
    for (size_t i = 0; i < file_name.length(); i++) {
      if (std::tolower(file_name[i]) != std::tolower(current_file_name[i])) {
        files_match = false;
        break;
      }
    }
    if (files_match) {
      return (working_directory / current_file_name).generic_string();
    }
  }

  SDL_Log("Could not find file %s", file_name.c_str());
  return file_name;
}
