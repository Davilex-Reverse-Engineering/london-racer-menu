#ifndef UI_BUTTON_HPP
#define UI_BUTTON_HPP

#include <string>

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "UiElement.hpp"

class UiButton : public UiElement {
public:
  UiButton(float x, float y, float width, float height, std::string text, bool selected);
  ~UiButton();

  void draw(SDL_Renderer * renderer);

private:
  SDL_FRect rect = {0.0f, 0.0f, 0.0f, 0.0f};
  std::string text_string = "";
  bool selected = false;
};

#endif // UI_BUTTON_HPP