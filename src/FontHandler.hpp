#ifndef FONT_HANDLER_HPP
#define FONT_HANDLER_HPP

#include "../vendor/SDL/include/SDL3/SDL.h"
#include "../vendor/SDL_ttf/include/SDL3_ttf/SDL_ttf.h"

#include <map>
#include <string>

enum class FontType {
  REGULAR,
  TITLE
};

class FontHandler {
public:
  FontHandler();
  ~FontHandler();

  bool load();
  static FontHandler * getInstance();

  SDL_Texture * getTexture(SDL_Renderer * renderer, std::string text, FontType font_type, SDL_Color color={255, 255, 255, 255});

private:
  std::map<FontType,TTF_Font *> fonts;

  const std::string sans_file_name = "resources/fonts/FreeSansBold.ttf";
};

#endif // FONT_HANDLER_HPP