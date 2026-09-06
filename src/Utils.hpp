#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

#include "../vendor/SDL/include/SDL3/SDL.h"

class Utils {
public:
  static std::string getFullPath(const std::string &file_name);
  static SDL_Texture * createTexture(SDL_Renderer * renderer, const std::string &file_name, bool use_transparency=false);
};

#endif // UTILS_HPP