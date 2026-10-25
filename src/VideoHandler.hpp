#ifndef VIDEO_HANDLER_HPP
#define VIDEO_HANDLER_HPP

#include "../vendor/SDL/include/SDL3/SDL.h"

#include <string>

class VideoHandler {
public:
  VideoHandler();
  ~VideoHandler();

  static VideoHandler * getInstance();

  SDL_Texture * getTexture(SDL_Renderer * renderer, const std::string &file_path);
};

#endif // VIDEO_HANDLER_HPP