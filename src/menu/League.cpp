#include "League.hpp"

#include "../ui/UiButton.hpp"
#include "../ui/UiRectangle.hpp"

League::League(int number)
{
  this->number = number;
  this->title = "Sunday Cup";

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

League::~League()
{

}

Action League::update(std::vector<Input> inputs)
{
  return this->processInputs(inputs);
}
