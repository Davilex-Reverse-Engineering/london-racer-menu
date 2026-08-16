#include "SundayCup.hpp"

#include "../ui/UiButton.hpp"
#include "../ui/UiRectangle.hpp"

SundayCup::SundayCup(IniHandler * game_ini_handler, IniHandler * static_ini_handler)
{
  this->game_ini_handler = game_ini_handler;
  this->static_ini_handler = static_ini_handler;
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

  this->text_laps = new UiText(464.0f, 394.0f, this->game_ini_handler->getValue("options", "nrlaps", "1"), {255, 255, 0, 255});
  this->ui_elements.push_back(this->text_laps);

  // Image for track preview
  std::string etappe = this->game_ini_handler->getValue("etappe", "etappe");
  std::string track_image_file_name = this->static_ini_handler->getValues("tracks", etappe)[2] + ".bmp";
  this->track_image = new UiImage(335.0f, 287.0f, 173.0f, 85.0f, track_image_file_name);
  this->ui_elements.push_back(this->track_image);

  this->selected = 1;
}

SundayCup::~SundayCup()
{

}

Action SundayCup::update(std::vector<Input> inputs)
{
  Action action = this->processInputs(inputs);
  switch (action) {
    case Action::INCREASE_LAPS:
      this->changeLaps(1);
      return Action::NONE;
      break;
    case Action::DECREASE_LAPS:
      this->changeLaps(-1);
      return Action::NONE;
      break;
    default:
      break;
  }
  return action;
}

void SundayCup::changeCar(int change)
{
}

void SundayCup::changeTrack(int change)
{
}

void SundayCup::changeLaps(int change)
{
  int laps = this->game_ini_handler->getInt("options", "nrlaps");
  std::string etappe = this->game_ini_handler->getValue("etappe", "etappe");
  bool has_laps = this->static_ini_handler->getValues("tracks", etappe)[4] == "0";
  if (!has_laps) {
    if (laps != 1) {
      this->game_ini_handler->setValue("options", "nrlaps", 1);
      text_laps->setText("1");
    }
    return;
  }

  laps += change;
  if (laps < 1) {
    laps = 1;
  } else if (laps > 5) {
    laps = 5;
  }
  this->game_ini_handler->setValue("options", "nrlaps", laps);
  text_laps->setText(std::to_string(laps));
}
