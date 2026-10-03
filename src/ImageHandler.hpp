#ifndef IMAGE_HANDLER_HPP
#define IMAGE_HANDLER_HPP

#include "../vendor/SDL/include/SDL3/SDL.h"

#include <unordered_map>
#include <string>

class ImageHandler {
public:
  ImageHandler();
  ~ImageHandler();

  static ImageHandler * getInstance();

  SDL_Texture * getTexture(SDL_Renderer * renderer, const std::string &file_path, bool use_transparency=false);

private:
  std::unordered_map<std::string,SDL_Texture *> textures;
};

#endif // IMAGE_HANDLER_HPP