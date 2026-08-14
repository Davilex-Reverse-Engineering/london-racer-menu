#include "Garage.hpp"

#include "../ui/UiButton.hpp"

Garage::Garage()
{
  this->title = "Garage";

  // Items are added from bottom left to top right
  this->ui_elements.push_back(new UiButton(10.0f, 415.0f, 100.0f, 50.0f, "Cancel", Action::OPEN_RACE_MENU));
  this->ui_elements.push_back(new UiButton(10.0f, 360.0f, 100.0f, 50.0f, "OK", Action::OK));

  this->selected = 0;
}

Garage::~Garage()
{
}
