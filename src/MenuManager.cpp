#include "MenuManager.hpp"

#include <typeinfo>

#include "menu/RaceMenu.hpp"
#include "menu/TournamentMenu.hpp"
#include "menu/SundayCup.hpp"
#include "menu/TimeTrialMenu.hpp"
#include "menu/Showroom.hpp"
#include "menu/Garage.hpp"
#include "menu/HallOfFame.hpp"
#include "menu/PlayerMenu.hpp"
#include "menu/Options.hpp"
#include "menu/Controls.hpp"
#include "menu/Settings.hpp"
#include "menu/TrackInfo.hpp"
#include "menu/League.hpp"

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
    case Action::OPEN_SHOWROOM:
      delete this->menu;
      this->menu = new Showroom();
      current_menu = Menu::SHOWROOM;
      break;
    case Action::OPEN_GARAGE:
      delete this->menu;
      this->menu = new Garage();
      current_menu = Menu::GARAGE;
      break;
    case Action::OPEN_HALL_OF_FAME:
      delete this->menu;
      this->menu = new HallOfFame(current_menu);
      current_menu = Menu::HALL_OF_FAME;
      break;
    case Action::OPEN_PLAYER_MENU:
      delete this->menu;
      this->menu = new Garage();
      current_menu = Menu::GARAGE;
      break;
    case Action::OPEN_OPTIONS:
      delete this->menu;
      this->menu = new Options(current_menu);
      current_menu = Menu::OPTIONS;
      break;
    case Action::OPEN_CONTROLS:
      delete this->menu;
      this->menu = new Controls();
      current_menu = Menu::CONTROLS;
      break;
    case Action::OPEN_SETTINGS:
      delete this->menu;
      this->menu = new Settings();
      current_menu = Menu::SETTINGS;
      break;
    case Action::OPEN_TRACK_INFO:
      delete this->menu;
      this->menu = new TrackInfo(current_menu);
      current_menu = Menu::TRACK_INFO;
      break;
    case Action::OPEN_LEAGUE_ONE:
      delete this->menu;
      this->menu = new League(1);
      current_menu = Menu::LEAGUE_ONE;
      break;
    case Action::OPEN_LEAGUE_TWO:
      delete this->menu;
      this->menu = new League(2);
      current_menu = Menu::LEAGUE_TWO;
      break;
    case Action::OPEN_LEAGUE_THREE:
      delete this->menu;
      this->menu = new League(3);
      current_menu = Menu::LEAGUE_THREE;
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
