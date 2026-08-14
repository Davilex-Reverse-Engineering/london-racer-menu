#include "InputManager.hpp"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

std::vector<Input> InputManager::getInputs() {
  std::vector<Input> inputs;
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    Input input = {
      .type = InputType::NONE,
      .event = InputEvent::NONE,
      .position = {0, 0},
    };

    switch (event.type) {
      case SDL_EVENT_QUIT:
        input.type = InputType::BUTTON;
        input.event = InputEvent::QUIT;
        break;
      case SDL_EVENT_KEY_DOWN:
        input.type = InputType::BUTTON;
        switch (event.key.key) {
          case SDLK_RETURN:
          case SDLK_SPACE:
            input.event = InputEvent::BUTTON_CONFIRM;
            break;
          case SDLK_A:
          case SDLK_S:
          case SDLK_LEFT:
          case SDLK_DOWN:
            input.event = InputEvent::BUTTON_LEFT;
            break;
          case SDLK_D:
          case SDLK_W:
          case SDLK_RIGHT:
          case SDLK_UP:
            input.event = InputEvent::BUTTON_RIGHT;
            break;
        default:
          input.event = InputEvent::NONE;
          break;
        }
        break;
      case SDL_EVENT_MOUSE_BUTTON_DOWN:
        input.type = InputType::POSITIONED;
        if (event.button.button == SDL_BUTTON_LEFT) {
          input.event = InputEvent::MOUSE_LEFT_PRESSED;
        } else {
          break;
        }
        SDL_GetMouseState(&input.position.x, &input.position.y);
        break;
      case SDL_EVENT_MOUSE_BUTTON_UP:
        input.type = InputType::POSITIONED;
        if (event.button.button == SDL_BUTTON_LEFT) {
          input.event = InputEvent::MOUSE_LEFT_RELEASED;
        } else {
          break;
        }
        SDL_GetMouseState(&input.position.x, &input.position.y);
        break;
      case SDL_EVENT_MOUSE_MOTION:
        input.type = InputType::POSITIONED;
        input.event = InputEvent::MOUSE_MOVE;
        SDL_GetMouseState(&input.position.x, &input.position.y);
        break;
    }
    if (input.type != InputType::NONE && input.event != InputEvent::NONE) {
        inputs.push_back(input);
    }
  }

  return inputs;
}
