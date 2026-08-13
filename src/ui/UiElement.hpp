#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "../Action.hpp"

class UiElement {
public:
  virtual ~UiElement() {};

  virtual void draw(SDL_Renderer * renderer, bool selected = false) = 0;

  bool isPointOnElement(SDL_FPoint * point)
  {
    return SDL_PointInRectFloat(point, &this->rect);
  };

  Action getAction() {return this->action;};
  bool isSelectable() {return this->selectable;};
protected:
  SDL_FRect rect = {0.0f, 0.0f, 0.0f, 0.0f};
  Action action = Action::NONE;
  bool selectable = false;
};

#endif // UI_ELEMENT_HPP