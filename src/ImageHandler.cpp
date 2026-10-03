#include "ImageHandler.hpp"

#include "Utils.hpp"

ImageHandler::ImageHandler()
{
}

ImageHandler::~ImageHandler()
{
  for (auto texture : this->textures)
  {
    if (texture.second) {
      SDL_DestroyTexture(texture.second);
    }
  }
}

ImageHandler * ImageHandler::getInstance()
{
  static ImageHandler instance;
  return &instance;
}

SDL_Texture * ImageHandler::getTexture(SDL_Renderer * renderer, const std::string &file_path, bool use_transparency)
{
  if (this->textures.contains(file_path)) {
    return this->textures[file_path];
  }

  std::string full_path = Utils::getFullPath(file_path);
  SDL_Surface * surface = SDL_LoadSurface(full_path.c_str());
  if (surface) {
    if (use_transparency) {
        uint32_t colorKey = SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), NULL, 0, 0, 0);
        SDL_SetSurfaceColorKey(surface, true, colorKey);
    }
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (texture) {
      this->textures[file_path] = texture;
    }
    return texture;
  }

  SDL_Log("Could not load image %s", full_path.c_str());
  return NULL;
}
