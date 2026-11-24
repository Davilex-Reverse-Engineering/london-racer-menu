#include "Settings.hpp"

#include "../ui/UiButton.hpp"
#include "../ui/UiRectangle.hpp"
#include "../ui/UiText.hpp"

Settings::Settings(IniHandler * game_ini_handler)
{
  this->game_ini_handler = game_ini_handler;
  this->title = "Settings";

  // Items are added from bottom left to top right
  this->ui_elements.push_back(new UiButton(10.0f, 415.0f, 100.0f, 50.0f, "Exit", Action::OPEN_OPTIONS));
  this->ui_elements.push_back(new UiButton(10.0f, 360.0f, 100.0f, 50.0f, "OK", Action::OK));

  this->ui_elements.push_back(new UiRectangle(215.0f, 75.0f, 414.0f, 24.0f, {120, 189, 34, 255}));
  this->ui_elements.push_back(new UiRectangle(215.0f, 99.0f, 414.0f, 141.0f, {71, 110, 23, 200}));

  this->ui_elements.push_back(new UiText(219.0f, 79.0f, "Audio", {255, 255, 255, 255}));

  this->checkbox_music = new UiCheckbox(243.0f, 131.0f, true, Action::SET_MUSIC);
  this->ui_elements.push_back(this->checkbox_music);

  this->checkbox_sound = new UiCheckbox(375.0f, 131.0f, true, Action::SET_SOUND);
  this->ui_elements.push_back(this->checkbox_sound);

  this->checkbox_police = new UiCheckbox(516.0f, 131.0f, true, Action::SET_POLICE_SOUND);
  this->ui_elements.push_back(this->checkbox_police);

  this->ui_elements.push_back(new UiRectangle(215.0f, 273.0f, 414.0f, 24.0f, {120, 189, 34, 255}));
  this->ui_elements.push_back(new UiRectangle(215.0f, 297.0f, 414.0f, 82.0f, {71, 110, 23, 200}));

  this->ui_elements.push_back(new UiText(219.0f, 278.0f, "Video", {255, 255, 255, 255}));

  this->selected = 0;
}

Settings::~Settings()
{
}

Action Settings::update(std::vector<Input> inputs)
{
  Action action = this->processInputs(inputs);
  switch (action) {
    case Action::SET_MUSIC:
      this->checkbox_music->setEnabled(!this->checkbox_music->getEnabled());
      return Action::NONE;
      break;
    case Action::SET_SOUND:
      this->checkbox_sound->setEnabled(!this->checkbox_sound->getEnabled());
      return Action::NONE;
      break;
    case Action::SET_POLICE_SOUND:
      this->checkbox_police->setEnabled(!this->checkbox_police->getEnabled());
      return Action::NONE;
      break;
    case Action::OK:

      return Action::OPEN_SETTINGS;
      break;
    default:
      break;
  }
  return action;
}