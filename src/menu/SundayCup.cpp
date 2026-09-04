#include "SundayCup.hpp"

#include "../constants.hpp"
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

  this->ui_elements.push_back(new UiButton(215.0f, 72.0f, 105.0f, 24.0f, "League 1", Action::CHANGE_LEAGUE_ONE));
  this->ui_elements.push_back(new UiButton(320.0f, 72.0f, 105.0f, 24.0f, "League 2", Action::CHANGE_LEAGUE_TWO));
  this->ui_elements.push_back(new UiButton(425.0f, 72.0f, 105.0f, 24.0f, "League 3", Action::CHANGE_LEAGUE_THREE));

  this->ui_elements.push_back(new UiButton(263.0f, 227.0f, 64.0f, 24.0f, "<", Action::CHANGE_CAR_LEFT));
  // this->ui_elements.push_back(new UiButton(380.0f, 227.0f, 80.0f, 32.0f, "Colour", Action::CHANGE_COLOR));
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

  this->laps = 2;
  this->text_laps = new UiText(464.0f, 394.0f, std::to_string(this->laps), {255, 255, 0, 255});
  this->ui_elements.push_back(this->text_laps);

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
  bool damage_enabled = true;
  this->game_ini_handler->setValue("options", "damage", damage_enabled);
  checkbox_damage = new UiCheckbox(593.0f, 283.0f, damage_enabled, Action::SET_DAMAGE);
  this->ui_elements.push_back(checkbox_damage);

  bool police_enabled = true;
  this->game_ini_handler->setValue("options", "police", police_enabled);
  checkbox_police = new UiCheckbox(593.0f, 323.0f, police_enabled, Action::SET_POLICE);
  this->ui_elements.push_back(checkbox_police);

  bool traffic_enabled = true;
  this->game_ini_handler->setValue("options", "traffic", traffic_enabled);
  checkbox_traffic = new UiCheckbox(593.0f, 363.0f, traffic_enabled, Action::SET_TRAFFIC);
  this->ui_elements.push_back(checkbox_traffic);

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
    case Action::SET_POLICE:
      this->checkbox_police->setEnabled(!this->checkbox_police->getEnabled());
      return Action::NONE;
      break;
    case Action::SET_TRAFFIC:
      this->checkbox_traffic->setEnabled(!this->checkbox_traffic->getEnabled());
      return Action::NONE;
      break;
    case Action::START:
      if (this->has_laps) {
        this->game_ini_handler->setValue("options", "nrlaps", this->laps);
      } else {
        this->game_ini_handler->setValue("options", "nrlaps", 1);
      }
      this->game_ini_handler->setValue("options", "opponents", true);
      this->game_ini_handler->setValue("options", "police", this->checkbox_police->getEnabled());
      this->game_ini_handler->setValue("options", "traffic", this->checkbox_traffic->getEnabled());
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

void SundayCup::changeCar(int change) {
  int car = this->game_ini_handler->getInt("player", "car");

  car += change;
  if (car < 0) {
    car = CAR_COUNT - 1;
  } else if (car >= CAR_COUNT - 1) {
    car = 0;
  }

  this->setCar(car);
}

void SundayCup::setCar(int car_id)
{
  std::string league_nr = this->game_ini_handler->getValue("league", "league");
  std::string car_name = this->static_ini_handler->getValues("league" + league_nr, "car" + std::to_string(car_id))[2];
  this->text_car->setText(car_name);
  this->game_ini_handler->setValue("player", "car", car_id);

}

void SundayCup::setTrack(int track)
{
  std::vector<std::string> track_values = this->static_ini_handler->getValues("tracks", std::to_string(track));
  std::string file_name = track_values[2] + ".bmp";
  this->track_image->setImage(file_name);

  this->has_laps = track_values[4] == "0";
  if (this->has_laps) {
    text_laps->setText(std::to_string(this->laps));
  } else {
    text_laps->setText("1");
  }
  this->game_ini_handler->setValue("etappe", "etappe", track + 1);
  this->track = track;
}

void SundayCup::changeTrack(int change)
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

void SundayCup::changeLaps(int change)
{
  if (!this->has_laps) {
    return;
  }
  this->laps += change;
  if (this->laps < 1) {
    this->laps = 1;
  } else if (this->laps > 5) {
    this->laps = 5;
  }
  text_laps->setText(std::to_string(this->laps));
}
