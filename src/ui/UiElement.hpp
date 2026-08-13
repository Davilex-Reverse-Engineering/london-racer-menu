#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

class UiElement {
public:
  virtual ~UiElement() {};

  virtual void draw(SDL_Renderer * renderer) = 0;
};

#endif // UI_ELEMENT_HPP