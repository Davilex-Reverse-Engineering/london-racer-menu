#include "FontHandler.hpp"

#include "Utils.hpp"

FontHandler::FontHandler()
{
}

FontHandler::~FontHandler()
{
  for (auto font : this->fonts)
  {
    if (font.second) {
      TTF_CloseFont(font.second);
    }
  }
}

bool FontHandler::load()
{
  if (!TTF_Init()) {
    SDL_Log("Could not initialize SDL_ttf: %s", SDL_GetError());
    return false;
  }

  std::string sans_path = Utils::getFullPath(sans_file_name);
  this->fonts[FontType::REGULAR] = TTF_OpenFont(sans_path.c_str(), 12);
  if (!this->fonts[FontType::REGULAR]) {
    SDL_Log("Could not load %s: %s", sans_path.c_str(), SDL_GetError());
    return false;
  }
  this->fonts[FontType::TITLE] = TTF_OpenFont(sans_path.c_str(), 24);
  if (!this->fonts[FontType::TITLE]) {
    SDL_Log("Could not load %s: %s", sans_path.c_str(), SDL_GetError());
    return false;
  }

  return true;
}

FontHandler * FontHandler::getInstance()
{
  static FontHandler instance;
  return &instance;
}

SDL_Texture * FontHandler::getTexture(SDL_Renderer * renderer, std::string text, FontType font_type, SDL_Color color)
{
  SDL_Surface * surface = TTF_RenderText_Blended(this->fonts[font_type], text.c_str(), text.length(), color);
  if (surface == NULL) {
      SDL_Log("Couldn't create surface for text %s: %s", text.c_str(), SDL_GetError());
      return NULL;
  }

  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);

  return texture;
}
