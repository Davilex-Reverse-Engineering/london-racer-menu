#include "Settings.hpp"

#include "../ui/UiButton.hpp"

Settings::Settings()
{
  this->title = "Settings";

  // Items are added from bottom left to top right
  this->ui_elements.push_back(new UiButton(10.0f, 415.0f, 100.0f, 50.0f, "Exit", Action::OPEN_RACE_MENU));

  this->selected = 0;
}

Settings::~Settings()
{
}
