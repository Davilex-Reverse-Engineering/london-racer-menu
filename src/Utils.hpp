#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

#include "../vendor/SDL/include/SDL3/SDL.h"

class Utils {
public:
  static std::string getFullPath(const std::string &file_name);
  static void * stringToWindows1252(std::string &input_string);
  static std::string  Windows1252ToString(void * windows_1252_string, size_t length);

  static bool validatedReservedBytes(SDL_IOStream * stream, size_t count);
  static bool readLeagueIdU32(SDL_IOStream * stream, uint32_t * league_id);
  static bool readLeagueIdU8(SDL_IOStream * stream, uint8_t * league_id);
  static bool readBoolean(SDL_IOStream * stream, bool * boolean);
};

#endif // UTILS_HPP