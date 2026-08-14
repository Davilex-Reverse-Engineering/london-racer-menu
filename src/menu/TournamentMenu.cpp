#include "TournamentMenu.hpp"

#include "../ui/UiButton.hpp"

TournamentMenu::TournamentMenu()
{
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
