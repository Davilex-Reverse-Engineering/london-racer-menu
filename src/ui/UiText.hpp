#ifndef UI_TEXT_HPP
#define UI_TEXT_HPP

#include <string>

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "UiElement.hpp"

class UiText : public UiElement {
public:
  UiText(float x, float y, std::string text, SDL_Color color);
  ~UiText();

  void draw(SDL_Renderer * renderer, bool selected = false);

private:
  std::string text_string = "";
  SDL_Color color = {255, 255, 255, 255};
};

#endif // UI_TEXT_HPP