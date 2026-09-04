#include "Results.hpp"

#include "../ui/UiButton.hpp"
#include "../ui/UiRectangle.hpp"
#include "../ui/UiImage.hpp"

Results::Results(IniHandler * game_ini_handler, IniHandler * static_ini_handler)
{
  this->game_ini_handler = game_ini_handler;
  this->static_ini_handler = static_ini_handler;
  this->title = "Results";

  // Items are added from bottom left to top right
  this->ui_elements.push_back(new UiButton(10.0f, 415.0f, 100.0f, 50.0f, "Continue", Action::OPEN_RACE_MENU));

  this->ui_elements.push_back(new UiRectangle(215.0f, 75.0f, 414.0f, 24.0f, {120, 189, 34, 255}));
  this->ui_elements.push_back(new UiRectangle(215.0f, 99.0f, 414.0f, 145.0f, {71, 110, 23, 200}));
  this->ui_elements.push_back(new UiRectangle(215.0f, 245.0f, 206.0f, 160.0f, {71, 110, 23, 200}));
  this->ui_elements.push_back(new UiRectangle(424.0f, 245.0f, 205.0f, 160.0f, {71, 110, 23, 200}));

  // Image for track preview
  int track = this->game_ini_handler->getInt("etappe", "etappe") - 1;
  std::vector<std::string> track_values = this->static_ini_handler->getValues("tracks", std::to_string(track));
  this->ui_elements.push_back(new UiImage(335.0f, 116.0f, 173.0f, 85.0f, track_values[2] + ".bmp"));

  this->selected = 0;
}

Results::~Results()
{
}
