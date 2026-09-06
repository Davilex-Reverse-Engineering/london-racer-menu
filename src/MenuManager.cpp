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
#include "menu/Results.hpp"

MenuManager::MenuManager(IniHandler * menu_ini_handler, IniHandler * game_ini_handler, IniHandler * static_ini_handler)
{
  this->menu_ini_handler = menu_ini_handler;
  this->game_ini_handler = game_ini_handler;
  this->static_ini_handler = static_ini_handler;
  if (this->menu_ini_handler->getBool("general", "firsttime")) {
    this->game_ini_handler->setValue("player", "finished", 0);
    this->current_menu = Menu::RACE_MENU;
    this->menu = new RaceMenu();
  } else {
    int previous = this->menu_ini_handler->getInt("general", "previous");
    if (this->game_ini_handler->getBool("player", "finished")) {
      this->current_menu = Menu::HALL_OF_FAME;
      switch (previous) {
        case 0:
          this->menu = new HallOfFame(game_ini_handler, static_ini_handler, Menu::SUNDAY_CUP);
          break;
        case 1:
          this->menu = new HallOfFame(game_ini_handler, static_ini_handler, Menu::TIME_TRIAL_MENU);
          break;
        case 2:
          this->menu = new HallOfFame(game_ini_handler, static_ini_handler, Menu::TIME_TRIAL_MENU);
          break;
        default:
          this->menu = new HallOfFame(game_ini_handler, static_ini_handler, Menu::RACE_MENU);
          break;
      }
    } else {
      switch (previous) {
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
          this->menu = new League(game_ini_handler, static_ini_handler);
          break;
      default:
        this->current_menu = Menu::RACE_MENU;
        this->menu = new RaceMenu();
        break;
      }
    }
  }
  this->loadMusic();
}

MenuManager::~MenuManager()
{
  if (this->background) {
    SDL_DestroyTexture(this->background);
  }
  if (this->menu) {
    delete this->menu;
  }
  if (this->music_buffer) {
    SDL_free(this->music_buffer);
  }
}

Action MenuManager::update(std::vector<Input> inputs)
{
  if (this->playing_music) {
    if (SDL_GetAudioStreamQueued(this->music_stream) < (int)this->music_length) {
      SDL_PutAudioStreamData(this->music_stream, this->music_buffer, this->music_length);
    }
  }
  Action action = Action::NONE;
  for(Input input : inputs) {
    if (input.event == InputEvent::QUIT) {
      action = Action::QUIT;
      break;
    }
  }
  if (action != Action::QUIT) {
    action = menu->update(inputs);
  }
  switch (action) {
    case Action::OPEN_RACE_MENU:
      this->game_ini_handler->setValue("player", "finished", 0);
      delete this->menu;
      this->menu = new RaceMenu();
      current_menu = Menu::RACE_MENU;
      break;
    case Action::OPEN_TOURNAMENT_MENU:
      this->game_ini_handler->setValue("player", "finished", 0);
      delete this->menu;
      this->menu = new TournamentMenu(game_ini_handler);
      current_menu = Menu::TOURNAMENT_MENU;
      break;
    case Action::OPEN_SUNDAY_CUP:
      this->game_ini_handler->setValue("player", "finished", 0);
      delete this->menu;
      this->menu = new SundayCup(game_ini_handler, static_ini_handler);
      current_menu = Menu::SUNDAY_CUP;
      break;
    case Action::OPEN_TIME_TRIAL_MENU:
      this->game_ini_handler->setValue("player", "finished", 0);
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
      this->menu = new HallOfFame(game_ini_handler, static_ini_handler, current_menu);
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
      this->menu = new League(game_ini_handler, static_ini_handler);
      current_menu = Menu::LEAGUE;
      break;
    case Action::START:
      this->menu_ini_handler->setValue("general", "exitcode", 40);
      this->setPreviousValue();
      return action;
    case Action::QUIT:
      this->menu_ini_handler->setValue("general", "exitcode", 255);
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

void MenuManager::loadMusic()
{
  int last = this->menu_ini_handler->getInt("music", "last");
  int count = this->menu_ini_handler->getInt("music", "count");
  int music = last + 1;
  if (music >= count) {
    music = 0;
  }
  std::string music_file_name = this->menu_ini_handler->getValue("music", std::to_string(music)) + ".wav";
  music_file_name = Utils::getFullPath(music_file_name);

  SDL_AudioSpec spec;
  SDL_LoadWAV(music_file_name.c_str(), &spec, &this->music_buffer, &this->music_length);

  this->music_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
  if (!this->music_stream) {
      SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
      return;
  }
  SDL_ResumeAudioStreamDevice(this->music_stream);

  this->menu_ini_handler->setValue("music", "last", music);
  playing_music = true;
}
