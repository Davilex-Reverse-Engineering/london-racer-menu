#include "TimeTrialMenu.hpp"

#include "../ui/UiButton.hpp"
#include "../ui/UiRectangle.hpp"

TimeTrialMenu::TimeTrialMenu(IniHandler * game_ini_handler, IniHandler * static_ini_handler)
{
  this->game_ini_handler = game_ini_handler;
  this->static_ini_handler = static_ini_handler;
  this->title = "Time Trial Menu";

  // Items are added from bottom left to top right
  this->ui_elements.push_back(new UiButton(10.0f, 415.0f, 100.0f, 50.0f, "Exit", Action::OPEN_RACE_MENU));
  this->ui_elements.push_back(new UiButton(10.0f, 360.0f, 100.0f, 50.0f, "Start", Action::START));
  this->ui_elements.push_back(new UiButton(10.0f, 305.0f, 100.0f, 50.0f, "Options", Action::OPEN_OPTIONS));
  this->ui_elements.push_back(new UiButton(10.0f, 250.0f, 100.0f, 50.0f, "Hall of Fame", Action::OPEN_HALL_OF_FAME));

  this->ui_elements.push_back(new UiRectangle(215.0f, 96.0f, 414.0f, 131.0f, {71, 110, 23, 200}));
  this->ui_elements.push_back(new UiRectangle(215.0f, 264.0f, 414.0f, 159.0f, {71, 110, 23, 200}));
    
  this->ui_elements.push_back(new UiButton(269.0f, 424.0f, 64.0f, 24.0f, "<", Action::CHANGE_TRACK_LEFT));
  this->ui_elements.push_back(new UiButton(384.0f, 424.0f, 80.0f, 32.0f, "Track Info", Action::OPEN_TRACK_INFO));
  this->ui_elements.push_back(new UiButton(510.0f, 424.0f, 64.0f, 24.0f, ">", Action::CHANGE_TRACK_RIGHT));
  
  this->ui_elements.push_back(new UiButton(263.0f, 227.0f, 64.0f, 24.0f, "<", Action::CHANGE_CAR_LEFT));
  this->ui_elements.push_back(new UiButton(380.0f, 227.0f, 80.0f, 32.0f, "Colour", Action::CHANGE_COLOR));
  this->ui_elements.push_back(new UiButton(516.0f, 227.0f, 64.0f, 24.0f, ">", Action::CHANGE_CAR_RIGHT));

  this->selected = 1;
}

TimeTrialMenu::~TimeTrialMenu()
{

}

Action TimeTrialMenu::update(std::vector<Input> inputs)
{
  Action action = this->processInputs(inputs);
  switch (action) {
    case Action::START:
      this->game_ini_handler->setValue("options", "opponents", false);
      this->game_ini_handler->setValue("options", "police", false);
      this->game_ini_handler->setValue("options", "traffic", false);
      this->game_ini_handler->setValue("options", "nrlaps", this->getNrLaps());
      this->game_ini_handler->setValue("player", "finished", 0);
      this->game_ini_handler->setValue("player", "damage", 0);
      this->game_ini_handler->setValue("player", "position", 4);
      this->game_ini_handler->setValue("player", "motor", true);
      this->game_ini_handler->setValue("player", "banden", true);
      this->game_ini_handler->setValue("player", "abs", true);
      this->game_ini_handler->setValue("player", "turbo", true);
      this->game_ini_handler->setValue("player", "versnellingsbak", true);
      return Action::START;
      break;
    default:
      break;
  }
  return action;
}

int TimeTrialMenu::getNrLaps()
{
  int track = this->game_ini_handler->getInt("etappe", "etappe") - 1;
  std::vector<std::string> track_values = this->static_ini_handler->getValues("tracks", std::to_string(track));

  int has_laps = track_values[4] == "0";
  if (has_laps) {
    return 999;
  }
  return 1;
}
