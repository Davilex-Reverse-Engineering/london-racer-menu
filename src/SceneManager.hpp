#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include <vector>

#include "../vendor/SDL/include/SDL3/SDL.h"

#include "scene/Scene.hpp"
#include "ui/UiElement.hpp"
#include "Action.hpp"
#include "Input.hpp"

class SceneManager {
public:
  SceneManager();
  ~SceneManager();

  Action update(std::vector<Input> inputs);
  void draw(SDL_Renderer *renderer);

private:
  Scene * scene = NULL;
  SDL_Texture * background = NULL;
  bool background_loaded = false;
};

#endif // SCENE_MANAGER_HPP