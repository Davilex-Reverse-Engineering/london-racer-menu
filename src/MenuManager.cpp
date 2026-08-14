#include "MenuManager.hpp"

#include <typeinfo>

#include "menu/RaceMenu.hpp"
#include "menu/TournamentMenu.hpp"
#include "menu/SundayCup.hpp"
#include "menu/TimeTrialMenu.hpp"
#include "menu/Options.hpp"

MenuManager::MenuManager()
{
  this->menu = new RaceMenu();
}

MenuManager::~MenuManager()
{
  if (this->background) {
    SDL_DestroyTexture(this->background);
  }
  if (this->menu) {
    delete this->menu;
  }
}

Action MenuManager::update(std::vector<Input> inputs)
{
  Action action = menu->update(inputs);
  switch (action) {
    case Action::OPEN_RACE_MENU:
      delete this->menu;
      this->menu = new RaceMenu();
      current_menu = Menu::RACE_MENU;
      break;
    case Action::OPEN_TOURNAMENT_MENU:
      delete this->menu;
      this->menu = new TournamentMenu();
      current_menu = Menu::TOURNAMENT_MENU;
      break;
    case Action::OPEN_SUNDAY_CUP:
      delete this->menu;
      this->menu = new SundayCup();
      current_menu = Menu::SUNDAY_CUP;
      break;
    case Action::OPEN_TIME_TRIAL_MENU:
      delete this->menu;
      this->menu = new TimeTrialMenu();
      current_menu = Menu::TIME_TRIAL_MENU;
      break;
    case Action::OPEN_OPTIONS:
      delete this->menu;
      this->menu = new Options(current_menu);
      current_menu = Menu::OPTIONS;
      break;
    case Action::START:
    case Action::QUIT:
      return action;
      break;
    default:
      break;
  }
  return Action::NONE;
}

void MenuManager::draw(SDL_Renderer *renderer)
{
  if (!this->background && !background_loaded) {
    SDL_Surface * background_surface = SDL_LoadBMP("BKGR.bmp");
    if (background_surface) {
      this->background = SDL_CreateTextureFromSurface(renderer, background_surface);
      SDL_DestroySurface(background_surface);
      if (!this->background) {
        SDL_Log("Could not load image BKGR.bmp as texture");
      }
    } else {
      SDL_Log("Could not load image BKGR.bmp");
    }
    background_loaded = true;
  }
  if (this->background) {
    SDL_RenderTexture(renderer, this->background, NULL, NULL);
  }
  menu->draw(renderer);
}
