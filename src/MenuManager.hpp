#ifndef MENU_MANAGER_HPP
#define MENU_MANAGER_HPP

#include <vector>

#include "../vendor/SDL/include/SDL3/SDL.h"

#include "menu/BaseMenu.hpp"
#include "ui/UiElement.hpp"
#include "Action.hpp"
#include "Input.hpp"

class MenuManager {
public:
  MenuManager();
  ~MenuManager();

  Action update(std::vector<Input> inputs);
  void draw(SDL_Renderer *renderer);

private:
  BaseMenu * menu = NULL;
  SDL_Texture * background = NULL;
  bool background_loaded = false;

  Menu current_menu = Menu::RACE_MENU;
};

#endif // MENU_MANAGER_HPP