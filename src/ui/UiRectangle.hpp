#ifndef UI_RECTANGLE_HPP
#define UI_RECTANGLE_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "UiElement.hpp"

class UiRectangle : public UiElement {
public:
  UiRectangle(float x, float y, float width, float height, SDL_Color color);
  ~UiRectangle();

  void draw(SDL_Renderer * renderer, bool selected = false);

private:
  SDL_Color color = {255, 255, 255, 255};
};

#endif // UI_RECTANGLE_HPP