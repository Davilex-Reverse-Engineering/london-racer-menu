#include "MenuManager.hpp"

#include <typeinfo>

#include "Utils.hpp"
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

MenuManager::MenuManager(IniHandler * menu_ini_handler, IniHandler * game_ini_handler, IniHandler * static_ini_handler)
{
  this->menu_ini_handler = menu_ini_handler;
  this->game_ini_handler = game_ini_handler;
  this->static_ini_handler = static_ini_handler;
  if (this->menu_ini_handler->getBool("general", "firsttime")) {
    this->current_menu = Menu::RACE_MENU;
    this->menu = new RaceMenu();
  } else {
    switch (this->menu_ini_handler->getInt("general", "previous")) {
      case 0:
        this->current_menu = Menu::SUNDAY_CUP;
        this->menu = new SundayCup(game_ini_handler, static_ini_handler);
        break;
      case 1:
        this->current_menu = Menu::TIME_TRIAL_MENU;
        this->menu = new TimeTrialMenu(game_ini_handler, static_ini_handler);
        break;
      case 2:
        this->current_menu = Menu::LEAGUE;
        this->menu = new League(this->game_ini_handler->getInt("league", "league"));
        break;
    default:
      this->current_menu = Menu::RACE_MENU;
      this->menu = new RaceMenu();
      break;
    }
  }
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
      this->menu = new TournamentMenu(game_ini_handler);
      current_menu = Menu::TOURNAMENT_MENU;
      break;
    case Action::OPEN_SUNDAY_CUP:
      delete this->menu;
      this->menu = new SundayCup(game_ini_handler, static_ini_handler);
      current_menu = Menu::SUNDAY_CUP;
      break;
    case Action::OPEN_TIME_TRIAL_MENU:
      delete this->menu;
      this->menu = new TimeTrialMenu(game_ini_handler, static_ini_handler);
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
      this->menu = new TrackInfo(current_menu, this->game_ini_handler, this->static_ini_handler);
      current_menu = Menu::TRACK_INFO;
      break;
    case Action::OPEN_LEAGUE:
      delete this->menu;
      this->menu = new League(this->game_ini_handler->getInt("league", "league"));
      current_menu = Menu::LEAGUE;
      break;
    case Action::START:
      this->setPreviousValue();
      return action;
    case Action::QUIT:
      return action;
      break;
    default:
      break;
  }
  return Action::NONE;
}

void MenuManager::draw(SDL_Renderer * renderer) {
  if (!this->background && !background_loaded) {
    std::string file_name = this->static_ini_handler->getValue("bitmaps", "0") + ".bmp";
    this->background = Utils::createTexture(renderer, file_name);
    background_loaded = true;
  }
  if (this->background) {
    SDL_RenderTexture(renderer, this->background, NULL, NULL);
  }
  menu->draw(renderer);
}

void MenuManager::setPreviousValue()
{
  switch (current_menu) {
    case Menu::SUNDAY_CUP:
      this->menu_ini_handler->setValue("general", "previous", 0);
      break;
    case Menu::TIME_TRIAL_MENU:
      this->menu_ini_handler->setValue("general", "previous", 1);
      break;
    case Menu::LEAGUE:
      this->menu_ini_handler->setValue("general", "previous", 2);
      break;
    default:
      break;
  }
}
