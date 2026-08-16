#include "HallOfFame.hpp"

#include "../ui/UiButton.hpp"

HallOfFame::HallOfFame(Menu last_menu)
{
  this->title = "Hall of Fame";

  // Items are added from bottom left to top right
  Action back_action = get_exit_action(last_menu);
  this->ui_elements.push_back(new UiButton(10.0f, 415.0f, 100.0f, 50.0f, "Back", back_action));

  this->selected = 0;
}

HallOfFame::~HallOfFame()
{
}

Action HallOfFame::get_exit_action(Menu last_menu)
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
    break;
  default:
    return Action::OPEN_RACE_MENU;
    break;
  }
}
