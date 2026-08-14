#include "SundayCup.hpp"

#include "../ui/UiButton.hpp"
#include "../ui/UiRectangle.hpp"

SundayCup::SundayCup()
{
  this->title = "Sunday Cup";

  // Items are added from bottom left to top right
  this->ui_elements.push_back(new UiButton(10.0f, 415.0f, 100.0f, 50.0f, "Exit", Action::OPEN_RACE_MENU));
  this->ui_elements.push_back(new UiButton(10.0f, 360.0f, 100.0f, 50.0f, "Start", Action::START));
  this->ui_elements.push_back(new UiButton(10.0f, 305.0f, 100.0f, 50.0f, "Options", Action::OPEN_OPTIONS));
  this->ui_elements.push_back(new UiButton(10.0f, 250.0f, 100.0f, 50.0f, "Hall of Fame", Action::OPEN_HALL_OF_FAME));

  this->ui_elements.push_back(new UiRectangle(215.0f, 96.0f, 414.0f, 131.0f, {71, 110, 23, 200}));
  this->ui_elements.push_back(new UiRectangle(215.0f, 264.0f, 414.0f, 159.0f, {71, 110, 23, 200}));

  this->ui_elements.push_back(new UiButton(263.0f, 227.0f, 64.0f, 24.0f, "<", Action::CHANGE_CAR_LEFT));
  this->ui_elements.push_back(new UiButton(380.0f, 227.0f, 80.0f, 32.0f, "Colour", Action::CHANGE_COLOR));
  this->ui_elements.push_back(new UiButton(516.0f, 227.0f, 64.0f, 24.0f, ">", Action::CHANGE_CAR_RIGHT));

  this->ui_elements.push_back(new UiButton(432.0f, 391.0f, 21.0f, 14.0f, "<", Action::DECREASE_LAPS));
  this->ui_elements.push_back(new UiButton(483.0f, 391.0f, 21.0f, 14.0f, ">", Action::INCREASE_LAPS));

  this->ui_elements.push_back(new UiButton(269.0f, 424.0f, 64.0f, 24.0f, "<", Action::CHANGE_TRACK_LEFT));
  this->ui_elements.push_back(new UiButton(384.0f, 424.0f, 80.0f, 32.0f, "Track Info", Action::OPEN_TRACK_INFO));
  this->ui_elements.push_back(new UiButton(510.0f, 424.0f, 64.0f, 24.0f, ">", Action::CHANGE_TRACK_RIGHT));

  // Texts
  this->ui_elements.push_back(new UiText(334.0f, 392.0f, "Nr of laps", {255, 255, 255, 255}));
  this->ui_elements.push_back(new UiText(519.0f, 292.0f, "Damage", {255, 255, 255, 255}));
  this->ui_elements.push_back(new UiText(535.0f, 330.0f, "Police", {255, 255, 255, 255}));
  this->ui_elements.push_back(new UiText(531.0f, 370.0f, "Traffic", {255, 255, 255, 255}));
  this->ui_elements.push_back(new UiText(525.0f, 174.0f, "Money", {255, 255, 255, 255}));
  this->ui_elements.push_back(new UiText(524.0f, 198.0f, "0", {255, 255, 0, 255}));

  text_laps = new UiText(464.0f, 394.0f, std::to_string(laps), {255, 255, 0, 255});
  this->ui_elements.push_back(text_laps);


  this->selected = 1;
}

SundayCup::~SundayCup()
{

}

Action SundayCup::update(std::vector<Input> inputs)
{
  Action action = this->processInputs(inputs);
  if (action == Action::DECREASE_LAPS) {
    laps--;
    if (laps < 1) {
      laps = 1;
    }
    text_laps->setText(std::to_string(laps));
    action = Action::NONE;
  } else if (action == Action::INCREASE_LAPS) {
    laps++;
    if (laps > 5) {
      laps = 5;
    }
    text_laps->setText(std::to_string(laps));
    action = Action::NONE;
  }
  return action;
}
