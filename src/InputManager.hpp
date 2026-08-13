#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include "../vendor/SDL/include/SDL3/SDL.h"

#include <vector>

#include "Input.hpp"

class InputManager {
public:
  InputManager();
  ~InputManager();

  std::vector<Input> getInputs();
};


#endif // INPUT_MANAGER_HPP