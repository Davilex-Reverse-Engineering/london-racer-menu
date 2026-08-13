#ifndef UI_BUTTON_HPP
#define UI_BUTTON_HPP

#include <string>

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "UiElement.hpp"

class UiButton : public UiElement {
public:
  UiButton(float x, float y, float width, float height, std::string text, Action action = Action::NONE);
  ~UiButton();

  void draw(SDL_Renderer * renderer, bool selected = false);

private:
  std::string text_string = "";
};

#endif // UI_BUTTON_HPP