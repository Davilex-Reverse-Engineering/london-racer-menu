#ifndef INPUT_HPP
#define INPUT_HPP

#include "../vendor/SDL/include/SDL3/SDL.h"

enum class InputType {
  NONE,
  POSITIONED,
  BUTTON,
};

enum class InputEvent {
  NONE,
  MOUSE_MOVE,
  MOUSE_LEFT_PRESSED,
  MOUSE_LEFT_RELEASED,
  BUTTON_CONFIRM,
  BUTTON_LEFT,
  BUTTON_RIGHT,
  QUIT
};

typedef struct {
  InputType type;
  InputEvent event;
  SDL_FPoint position;
} Input;

#endif // INPUT_HPP