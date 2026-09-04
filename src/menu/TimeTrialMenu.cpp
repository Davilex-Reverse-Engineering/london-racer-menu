#include "TimeTrialMenu.hpp"

#include "../constants.hpp"
#include "../ui/UiButton.hpp"
#include "../ui/UiRectangle.hpp"

TimeTrialMenu::TimeTrialMenu(IniHandler * game_ini_handler, IniHandler * static_ini_handler)
{
  this->game_ini_handler = game_ini_handler;
  this->static_ini_handler = static_ini_handler;
  this->title = "Time Trial Menu";

  // Items are added from bottom left to top right
  this->ui_elements.push_back(new UiButton(10.0f, 415.0f, 100.0f, 50.0f, "Exit", Action::OPEN_RACE_MENU));
  this->ui_elements.push_back(new UiButton(10.0f, 360.0f, 100.0f, 50.0f, "Start", Action::START));
  this->ui_elements.push_back(new UiButton(10.0f, 305.0f, 100.0f, 50.0f, "Options", Action::OPEN_OPTIONS));
  this->ui_elements.push_back(new UiButton(10.0f, 250.0f, 100.0f, 50.0f, "Hall of Fame", Action::OPEN_HALL_OF_FAME));

  this->ui_elements.push_back(new UiRectangle(215.0f, 96.0f, 414.0f, 131.0f, {71, 110, 23, 200}));
  this->ui_elements.push_back(new UiRectangle(215.0f, 264.0f, 414.0f, 159.0f, {71, 110, 23, 200}));

  this->ui_elements.push_back(new UiButton(215.0f, 72.0f, 105.0f, 24.0f, "League 1", Action::CHANGE_LEAGUE_ONE));
  this->ui_elements.push_back(new UiButton(320.0f, 72.0f, 105.0f, 24.0f, "League 2", Action::CHANGE_LEAGUE_TWO));
  this->ui_elements.push_back(new UiButton(425.0f, 72.0f, 105.0f, 24.0f, "League 3", Action::CHANGE_LEAGUE_THREE));

  this->ui_elements.push_back(new UiButton(263.0f, 227.0f, 64.0f, 24.0f, "<", Action::CHANGE_CAR_LEFT));
  // this->ui_elements.push_back(new UiButton(380.0f, 227.0f, 80.0f, 32.0f, "Colour", Action::CHANGE_COLOR));
  this->ui_elements.push_back(new UiButton(516.0f, 227.0f, 64.0f, 24.0f, ">", Action::CHANGE_CAR_RIGHT));

  this->ui_elements.push_back(new UiButton(269.0f, 424.0f, 64.0f, 24.0f, "<", Action::CHANGE_TRACK_LEFT));
  this->ui_elements.push_back(new UiButton(384.0f, 424.0f, 80.0f, 32.0f, "Track Info", Action::OPEN_TRACK_INFO));
  this->ui_elements.push_back(new UiButton(510.0f, 424.0f, 64.0f, 24.0f, ">", Action::CHANGE_TRACK_RIGHT));

  // Texts
  this->ui_elements.push_back(new UiText(519.0f, 292.0f, "Damage", {255, 255, 255, 255}));

  // Car selection
  std::string car_nr = this->game_ini_handler->getValue("player", "car");
  std::string league_nr = this->game_ini_handler->getValue("league", "league");
  std::string car_name = this->static_ini_handler->getValues("league" + league_nr, "car" + car_nr)[2];
  this->text_car = new UiText(330.0f, 150.0f, car_name, {255, 255, 255, 255});
  this->ui_elements.push_back(this->text_car);

  // Image for track preview
  int track = this->game_ini_handler->getInt("etappe", "etappe") - 1;
  this->track_image = new UiImage(335.0f, 287.0f, 173.0f, 85.0f);
  this->setTrack(track);
  this->ui_elements.push_back(this->track_image);

  // Checkboxes
  bool damage_enabled = false;
  this->game_ini_handler->setValue("options", "damage", damage_enabled);
  checkbox_damage = new UiCheckbox(593.0f, 283.0f, damage_enabled, Action::SET_DAMAGE);
  this->ui_elements.push_back(checkbox_damage);

  this->selected = 1;
}

TimeTrialMenu::~TimeTrialMenu()
{

}

Action TimeTrialMenu::update(std::vector<Input> inputs)
{
  Action action = this->processInputs(inputs);
  switch (action) {
    case Action::CHANGE_TRACK_LEFT:
      this->changeTrack(-1);
      return Action::NONE;
      break;
    case Action::CHANGE_TRACK_RIGHT:
      this->changeTrack(1);
      return Action::NONE;
      break;
    case Action::CHANGE_CAR_LEFT:
      this->changeCar(-1);
      return Action::NONE;
      break;
    case Action::CHANGE_CAR_RIGHT:
      this->changeCar(1);
      return Action::NONE;
      break;
    case Action::CHANGE_LEAGUE_ONE:
      this->game_ini_handler->setValue("league", "league", 0);
      this->setCar(0);
      return Action::NONE;
    case Action::CHANGE_LEAGUE_TWO:
      this->game_ini_handler->setValue("league", "league", 1);
      this->setCar(0);
      return Action::NONE;
    case Action::CHANGE_LEAGUE_THREE:
      this->game_ini_handler->setValue("league", "league", 2);
      this->setCar(0);
      return Action::NONE;
    case Action::SET_DAMAGE:
      this->checkbox_damage->setEnabled(!this->checkbox_damage->getEnabled());
      return Action::NONE;
      break;
    case Action::START:
      this->game_ini_handler->setValue("options", "opponents", false);
      this->game_ini_handler->setValue("options", "police", false);
      this->game_ini_handler->setValue("options", "traffic", false);
      this->game_ini_handler->setValue("options", "nrlaps", this->getNrLaps());
      this->game_ini_handler->setValue("options", "invulnerable", !this->checkbox_damage->getEnabled());
      this->game_ini_handler->setValue("player", "finished", 0);
      this->game_ini_handler->setValue("player", "damage", 0);
      this->game_ini_handler->setValue("player", "position", 4);
      this->game_ini_handler->setValue("player", "motor", true);
      this->game_ini_handler->setValue("player", "banden", true);
      this->game_ini_handler->setValue("player", "abs", true);
      this->game_ini_handler->setValue("player", "turbo", true);
      this->game_ini_handler->setValue("player", "versnellingsbak", true);
      return Action::START;
      break;
    default:
      break;
  }
  return action;
}

int TimeTrialMenu::getNrLaps()
{
  int track = this->game_ini_handler->getInt("etappe", "etappe") - 1;
  std::vector<std::string> track_values = this->static_ini_handler->getValues("tracks", std::to_string(track));

  int has_laps = track_values[4] == "0";
  if (has_laps) {
    return 999;
  }
  return 1;
}

void TimeTrialMenu::changeCar(int change)
{
  int car = this->game_ini_handler->getInt("player", "car");

  car += change;
  if (car < 0) {
    car = CAR_COUNT - 1;
  } else if (car >= CAR_COUNT - 1) {
    car = 0;
  }

  this->setCar(car);
}

void TimeTrialMenu::setCar(int car_id)
{
  std::string league_nr = this->game_ini_handler->getValue("league", "league");
  std::string car_name = this->static_ini_handler->getValues("league" + league_nr, "car" + std::to_string(car_id))[2];
  this->text_car->setText(car_name);
  this->game_ini_handler->setValue("player", "car", car_id);
}

void TimeTrialMenu::setTrack(int track)
{
  std::vector<std::string> track_values = this->static_ini_handler->getValues("tracks", std::to_string(track));
  std::string file_name = track_values[2] + ".bmp";
  this->track_image->setImage(file_name);

  this->game_ini_handler->setValue("etappe", "etappe", track + 1);
  this->track = track;
}

void TimeTrialMenu::changeTrack(int change)
{
  int track_count = this->static_ini_handler->getInt("tracks", "count");
  int track = this->track + change;
  if (track < 0) {
    track = track_count - 1;
  } else if (track >= track_count - 1) {
    track = 0;
  }
  this->setTrack(track);
}
