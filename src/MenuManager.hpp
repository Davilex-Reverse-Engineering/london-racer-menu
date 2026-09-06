#ifndef MENU_MANAGER_HPP
#define MENU_MANAGER_HPP

#include <vector>

#include "../vendor/SDL/include/SDL3/SDL.h"

#include "menu/BaseMenu.hpp"
#include "ui/UiElement.hpp"
#include "Action.hpp"
#include "Input.hpp"
#include "IniHandler.hpp"

class MenuManager {
public:
  MenuManager(IniHandler * menu_ini_handler, IniHandler * game_ini_handler,  IniHandler * static_ini_handler);
  ~MenuManager();

  Action update(std::vector<Input> inputs);
  void draw(SDL_Renderer *renderer);

private:
  BaseMenu * menu = NULL;
  SDL_Texture * background = NULL;
  bool background_loaded = false;

  IniHandler * menu_ini_handler = NULL;
  IniHandler * game_ini_handler = NULL;
  IniHandler * static_ini_handler = NULL;

  Menu current_menu = Menu::RACE_MENU;

  uint8_t * music_buffer = NULL;
  uint32_t music_length = 0;
  SDL_AudioStream * music_stream = NULL;

  bool playing_music = false;

  void setPreviousValue();
  void loadMusic();

  Menu getLastMenuFromPrevious();
};

#endif // MENU_MANAGER_HPP