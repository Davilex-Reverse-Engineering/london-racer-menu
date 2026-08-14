#include "Options.hpp"

#include "../ui/UiButton.hpp"

Options::Options(Menu last_menu)
{
  this->title = "Options";
  Action back_action = this->get_exit_action(last_menu);

  // Items are added from bottom left to top right
  this->ui_elements.push_back(new UiButton(10.0f, 415.0f, 100.0f, 50.0f, "Back", back_action));
  this->ui_elements.push_back(new UiButton(10.0f, 360.0f, 100.0f, 50.0f, "Player", Action::OPEN_PLAYER_MENU));
  this->ui_elements.push_back(new UiButton(10.0f, 305.0f, 100.0f, 50.0f, "Settings", Action::OPEN_SETTINGS));
  this->ui_elements.push_back(new UiButton(10.0f, 250.0f, 100.0f, 50.0f, "Controls", Action::OPEN_CONTROLS));

  this->selected = 3;
}

Options::~Options()
{
}

Action Options::update(std::vector<Input> inputs)
{
  return this->processInputs(inputs); 
}

Action Options::get_exit_action(Menu last_menu)
{
  switch (last_menu) {
    case Menu::RACE_MENU:
      return Action::OPEN_RACE_MENU;
      break;
    case Menu::TOURNAMENT_MENU:
      return Action::OPEN_TOURNAMENT_MENU;
      break;
    case Menu::SUNDAY_CUP:
      return Action::OPEN_SUNDAY_CUP;
      break;
    case Menu::TIME_TRIAL_MENU:
      return Action::OPEN_TIME_TRIAL_MENU;
      break;
    case Menu::LEAGUE_ONE:
      return Action::OPEN_LEAGUE_ONE;
      break;
    case Menu::LEAGUE_TWO:
      return Action::OPEN_LEAGUE_TWO;
      break;
    case Menu::LEAGUE_THREE:
      return Action::OPEN_LEAGUE_THREE;
      break;
    case Menu::OPTIONS:
      return Action::OPEN_OPTIONS;
      break;
    case Menu::SETTINGS:
    case Menu::CONTROLS:
    case Menu::TRACK_INFO:
    case Menu::PLAYER_MENU:
    case Menu::HALL_OF_FAME:
    case Menu::GARAGE:
    case Menu::SHOWROOM:
      SDL_Log("Invalid return menu for exit button found! Please fix this!");
      break;
  }
  return Action::OPEN_RACE_MENU;
}
