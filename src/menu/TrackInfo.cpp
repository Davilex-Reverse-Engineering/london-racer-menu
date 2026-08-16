#include "TrackInfo.hpp"

#include "../ui/UiImage.hpp"
#include "../ui/UiButton.hpp"

TrackInfo::TrackInfo(Menu last_menu, IniHandler * game_ini_handler, IniHandler * static_ini_handler)
{
  this->title = "Track Info";

  std::string background_image_file_name = static_ini_handler->getValue("bitmaps", "6") + ".bmp";
  this->ui_elements.push_back(new UiImage(0.0f, 0.0f, 640.0f, 480.0f, background_image_file_name));

  std::string etappe = game_ini_handler->getValue("etappe", "etappe");
  std::string track_image_file_name = static_ini_handler->getValues("tracks", etappe)[3] + ".bmp";
  this->ui_elements.push_back(new UiImage(100.0f, 140.0f, 474.0f, 220.0f, track_image_file_name));

  this->ui_elements.push_back(new UiButton(10.0f, 415.0f, 100.0f, 50.0f, "Exit", get_exit_action(last_menu)));

  this->selected = 0;
}

TrackInfo::~TrackInfo()
{
}

Action TrackInfo::get_exit_action(Menu last_menu)
{
  switch (last_menu) {
  case Menu::SUNDAY_CUP:
    return Action::OPEN_SUNDAY_CUP;
    break;
  case Menu::TIME_TRIAL_MENU:
    return Action::OPEN_TIME_TRIAL_MENU;
    break;
  case Menu::LEAGUE:
    return Action::OPEN_LEAGUE;
  default:
    return Action::OPEN_RACE_MENU;
    break;
  }
}
