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

std::string Utils::Windows1252ToString(void * windows_1252_string, size_t length)
{
  SDL_iconv_t iconv = SDL_iconv_open("UTF-8", "ISO-8859-1");
  if ((size_t)iconv == SDL_ICONV_ERROR) {
    SDL_Log("Failed to start iconv, ISO-8859-1 support might not be loadable");
    return "";
  }
  void * original = windows_1252_string;
  size_t inbytesleft = length;
  size_t outbytesleft = length * 3;
  char * target = (char *) SDL_calloc(length, 3);  // 3 is probably overkill tbh
  char * target_start = target;

  size_t iconv_result = SDL_iconv(iconv, (const char **)&original, &inbytesleft, &target, &outbytesleft);
  if (iconv_result == 0) {
    std::string result = std::string((char *)target_start, length);
    free(target_start);
    SDL_iconv_close(iconv);
    return result;
  }

  SDL_Log("Failed convert with status %u (%i)", iconv_result, iconv_result);
  free(target_start);
  SDL_iconv_close(iconv);
  return "";
}

void * Utils::stringToWindows1252(std::string &input_string)
{
  SDL_iconv_t iconv = SDL_iconv_open("ISO-8859-1", "UTF-8");
  if ((size_t)iconv == SDL_ICONV_ERROR) {
    SDL_Log("Failed to start iconv, ISO-8859-1 support might not be loadable");
    return NULL;
  }

  const char * original = input_string.c_str();
  size_t inbytesleft = input_string.length();
  size_t outbytesleft = 32; // Names are always 32 bytes in total
  void * target = SDL_calloc(outbytesleft, 1);
  void * target_start = target;

  size_t iconv_result = SDL_iconv(iconv, &original, &inbytesleft, (char **)&target, &outbytesleft);
  if (iconv_result == 0) {
    SDL_iconv_close(iconv);
    return target_start;
  }

  SDL_Log("Failed convert with status %u (%i)", iconv_result, iconv_result);
  SDL_iconv_close(iconv);
  free(target_start);
  return NULL;
}
