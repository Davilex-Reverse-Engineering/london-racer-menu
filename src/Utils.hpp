#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

#include "../vendor/SDL/include/SDL3/SDL.h"

class Utils {
public:
  static std::string getFullPath(const std::string &file_name);
};

#endif // UTILS_HPP