#include "TournamentMenu.hpp"

#include "../ui/UiButton.hpp"

TournamentMenu::TournamentMenu(IniHandler * game_ini_handler)
{
  this->game_ini_handler = game_ini_handler;

  this->title = "Tournament Menu";

  // Items are added from bottom left to top right
  this->ui_elements.push_back(new UiButton(10.0f, 415.0f, 100.0f, 50.0f, "Exit", Action::OPEN_RACE_MENU));
  this->ui_elements.push_back(new UiButton(10.0f, 360.0f, 100.0f, 50.0f, "Options", Action::OPEN_OPTIONS));
  this->ui_elements.push_back(new UiButton(343.0f, 243.0f, 100.0f, 50.0f, "League 3", Action::OPEN_LEAGUE_THREE));
  this->ui_elements.push_back(new UiButton(343.0f, 193.0f, 100.0f, 50.0f, "League 2", Action::OPEN_LEAGUE_TWO));
  this->ui_elements.push_back(new UiButton(343.0f, 143.0f, 100.0f, 50.0f, "League 1", Action::OPEN_LEAGUE_ONE));
  this->selected = 4;
}

TournamentMenu::~TournamentMenu()
{
}

Action TournamentMenu::update(std::vector<Input> inputs)
{
  Action action = this->processInputs(inputs);
  switch (action) {
    case Action::OPEN_LEAGUE_ONE:
      this->game_ini_handler->setValue("league", "league", 0);
      return Action::OPEN_LEAGUE;
      break;
    case Action::OPEN_LEAGUE_TWO:
      this->game_ini_handler->setValue("league", "league", 1);
      return Action::OPEN_LEAGUE;
      break;
    case Action::OPEN_LEAGUE_THREE:
      this->game_ini_handler->setValue("league", "league", 2);
      return Action::OPEN_LEAGUE;
      break;
    default:
      return action;
      break;
  }
}