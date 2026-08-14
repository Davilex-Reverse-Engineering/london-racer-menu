#include "RaceMenu.hpp"

#include "../ui/UiButton.hpp"

RaceMenu::RaceMenu()
{
  this->title = "Race Menu";

  // Items are added from bottom left to top right
  this->ui_elements.push_back(new UiButton(10.0f, 415.0f, 100.0f, 50.0f, "Quit", Action::QUIT));
  this->ui_elements.push_back(new UiButton(10.0f, 360.0f, 100.0f, 50.0f, "Options", Action::OPEN_OPTIONS));
  this->ui_elements.push_back(new UiButton(215.0f, 158.0f, 100.0f, 50.0f, "Sunday Cup", Action::OPEN_SUNDAY_CUP));
  this->ui_elements.push_back(new UiButton(215.0f, 102.0f, 100.0f, 50.0f, "Time Trial", Action::OPEN_TIME_TRIAL_MENU));
  this->ui_elements.push_back(new UiButton(329.0f, 103.0f, 300.0f, 104.0f, "Tournament", Action::OPEN_TOURNAMENT_MENU));
  this->selected = 4;
}

RaceMenu::~RaceMenu()
{

}

Action RaceMenu::update(std::vector<Input> inputs)
{
  return this->processInputs(inputs);
}
