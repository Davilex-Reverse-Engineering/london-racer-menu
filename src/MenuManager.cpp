#include "MenuManager.hpp"

#include "menu/RaceMenu.hpp"
#include "menu/TournamentMenu.hpp"
#include "menu/SundayCup.hpp"

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
      break;
    case Action::OPEN_TOURNAMENT_MENU:
      delete this->menu;
      this->menu = new TournamentMenu();
      break;
    case Action::OPEN_SUNDAY_CUP:
      delete this->menu;
      this->menu = new SundayCup();
      break;
    case Action::QUIT:
      return action;
      break;
  default:
    break;
  }
  if (action == Action::QUIT) {
    return action;
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
