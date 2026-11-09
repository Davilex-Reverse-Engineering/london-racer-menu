#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include "../vendor/SDL/include/SDL3/SDL.h"

#include <vector>

#include "Input.hpp"

class InputManager {
public:
  InputManager(SDL_Renderer * renderer);
  ~InputManager();

  std::vector<Input> getInputs();

private:
  SDL_Renderer * renderer;
};


#endif // INPUT_MANAGER_HPP