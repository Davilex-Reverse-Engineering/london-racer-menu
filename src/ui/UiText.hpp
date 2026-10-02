#ifndef UI_TEXT_HPP
#define UI_TEXT_HPP

#include <string>

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "UiElement.hpp"
#include "../FontHandler.hpp"

class UiText : public UiElement {
public:
  UiText(float x, float y, std::string text);
  UiText(float x, float y, std::string text, SDL_Color color);
  UiText(float x, float y, std::string text, SDL_Color color, FontType font_type);
  ~UiText();

  void draw(SDL_Renderer * renderer, bool selected = false);

  void setText(const std::string &text);
private:
  std::string text_string = "";
  SDL_Color color = {255, 255, 255, 255};
  FontType font_type = FontType::REGULAR;
  SDL_Texture * texture = NULL;
};

#endif // UI_TEXT_HPP