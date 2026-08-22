#ifndef UI_CHECKBOX_HPP
#define UI_CHECKBOX_HPP

#include <string>

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "UiElement.hpp"

class UiCheckbox : public UiElement {
public:
  UiCheckbox(float x, float y, bool enabled, Action action = Action::NONE);
  ~UiCheckbox();

  void draw(SDL_Renderer * renderer, bool selected = false);

  bool getEnabled() {return this->enabled;};
  void setEnabled(bool enabled) {this->enabled = enabled;};
private:
  bool enabled = false;
};

#endif // UI_CHECKBOX_HPP