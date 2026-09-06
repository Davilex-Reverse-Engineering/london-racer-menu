#include "HallOfFame.hpp"

#include <string>

#include "../ui/UiButton.hpp"
#include "../ui/UiRectangle.hpp"

HallOfFame::HallOfFame(IniHandler * game_ini_handler, IniHandler * static_ini_handler, Menu last_menu)
{
  this->game_ini_handler = game_ini_handler;
  this->static_ini_handler = static_ini_handler;
  this->scores_handler.load();
  this->title = "Hall of Fame";

  // Items are added from bottom left to top right
  Action back_action = get_exit_action(last_menu);
  this->ui_elements.push_back(new UiButton(10.0f, 415.0f, 100.0f, 50.0f, "Back", back_action));

  this->ui_elements.push_back(new UiRectangle(215.0f, 75.0f, 414.0f, 24.0f, {120, 189, 34, 255}));
  this->ui_elements.push_back(new UiRectangle(215.0f, 99.0f, 414.0f, 122.0f, {71, 110, 23, 200}));

  this->ui_elements.push_back(new UiRectangle(215.0f, 295.0f, 414.0f, 165.0f, {71, 110, 23, 200}));
  
  this->ui_elements.push_back(new UiText(219.0f, 299.0f, "Best laps", {255, 255, 255, 255}));
  this->ui_elements.push_back(new UiText(219.0f, 379.0f, "Best totals", {255, 255, 255, 255}));

  // Image for track preview
  this->load_track_image();
  this->load_records();

  this->ui_elements.push_back(new UiButton(270.0f, 224.0f, 64.0f, 24.0f, "<", Action::CHANGE_TRACK_LEFT));
  this->ui_elements.push_back(new UiButton(510.0f, 224.0f, 64.0f, 24.0f, ">", Action::CHANGE_TRACK_RIGHT));


  this->ui_elements.push_back(new UiButton(215.0f, 271.0f, 105.0f, 24.0f, "League 1", Action::CHANGE_LEAGUE_ONE));
  this->ui_elements.push_back(new UiButton(320.0f, 271.0f, 105.0f, 24.0f, "League 2", Action::CHANGE_LEAGUE_TWO));
  this->ui_elements.push_back(new UiButton(425.0f, 271.0f, 105.0f, 24.0f, "League 3", Action::CHANGE_LEAGUE_THREE));

  this->selected = 0;
}

HallOfFame::~HallOfFame()
{
}

Action HallOfFame::update(std::vector<Input> inputs)
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
    case Action::CHANGE_LEAGUE_ONE:
      this->game_ini_handler->setValue("league", "league", 0);
      this->load_records();
      break;
    case Action::CHANGE_LEAGUE_TWO:
      this->game_ini_handler->setValue("league", "league", 1);
      this->load_records();
      break;
    case Action::CHANGE_LEAGUE_THREE:
      this->game_ini_handler->setValue("league", "league", 2);
      this->load_records();
      break;
    default:
      break;
  }
  return action;
}

void HallOfFame::changeTrack(int change)
{
  int etappe = this->game_ini_handler->getInt("etappe", "etappe");
  int track_count = this->static_ini_handler->getInt("tracks", "count");

  etappe += change;
  if (etappe < 1) {
    etappe = track_count - 1;
  } else if (etappe > track_count) {
    etappe = 1;
  }
  this->game_ini_handler->setValue("etappe", "etappe", etappe);
  this->load_track_image();
  this->load_records();
}

void HallOfFame::load_track_image()
{
  std::string etappe = this->game_ini_handler->getValue("etappe", "etappe");
  int track_count = this->static_ini_handler->getInt("tracks", "count");
  for (int i = 0; i < track_count; i++) {
    std::vector<std::string> track_entries = this->static_ini_handler->getValues("tracks", std::to_string(i));
    if (etappe == track_entries[0]) {
      this->track = i;
      break;
    }
  }
  std::vector<std::string> track_entries = this->static_ini_handler->getValues("tracks", std::to_string(this->track));
  std::string image = track_entries[2] + ".bmp";
  if (this->track_image == NULL) {
    this->track_image = new UiImage(335.0f, 116.0f, 173.0f, 85.0f, image);
    this->ui_elements.push_back(this->track_image);
  } else {
    this->track_image->setImage(image);
  }
}

void HallOfFame::load_records()
{
  int league = this->game_ini_handler->getInt("league", "league");
  std::vector<Record> * lap_records = this->scores_handler.getLapRecords(this->track, league);
  for(size_t i = 0; i < lap_records->size(); i++) {
    if (this->laps_names.size() != lap_records->size()) {
      UiText * text = new UiText(235.0f, 323.0f + (18.0f * (float) i), (*lap_records)[i].name, {255, 255, 255, 255});
      this->laps_names.push_back(text);
      this->ui_elements.push_back(text);
    } else {
      this->laps_names[i]->setText((*lap_records)[i].name);
    }

    std::string time = ScoresHandler::getTimeString((*lap_records)[i].time_in_ms);
    if (this->laps_times.size() != lap_records->size()) {
      UiText * text = new UiText(484.0f, 323.0f + (18.0f * (float) i), time, {255, 255, 255, 255});
      laps_times.push_back(text);
      this->ui_elements.push_back(text);
    } else {
      this->laps_times[i]->setText(time);
    }

    std::vector<std::string> car_values = this->static_ini_handler->getValues("league" + std::to_string(league), "car" + std::to_string((*lap_records)[i].car));
    std::string car_image_bmp = car_values[2] + ".bmp";
    if (this->laps_car_images.size() != lap_records->size()) {
      UiImage * image = new UiImage(384.0f, 320.0f + (18.0f * (float) i), 60.0f, 18.0f, car_image_bmp, true);
      this->laps_car_images.push_back(image);
      this->ui_elements.push_back(image);
    } else {
      this->laps_car_images[i]->setImage(car_image_bmp);
    }
  }

  std::vector<Record> * total_records = this->scores_handler.getLapRecords(this->track, league);
  for(size_t i = 0; i < total_records->size(); i++) {
    if (this->totals_names.size() != total_records->size()) {
      UiText * text = new UiText(235.0f, 403.0f + (18.0f * (float) i), (*total_records)[i].name, {255, 255, 255, 255});
      this->totals_names.push_back(text);
      this->ui_elements.push_back(text);
    } else {
      this->totals_names[i]->setText((*total_records)[i].name);
    }

    std::string time = ScoresHandler::getTimeString((*total_records)[i].time_in_ms);
    if (this->totals_times.size() != total_records->size()) {
      UiText * text = new UiText(484.0f, 403.0f + (18.0f * (float) i), time, {255, 255, 255, 255});
      totals_times.push_back(text);
      this->ui_elements.push_back(text);
    } else {
      this->totals_times[i]->setText(time);
    }

    std::vector<std::string> car_values = this->static_ini_handler->getValues("league" + std::to_string(league), "car" + std::to_string((*total_records)[i].car));
    std::string car_image_bmp = car_values[2] + ".bmp";
    if (this->totals_car_images.size() != total_records->size()) {
      UiImage * image = new UiImage(384.0f, 400.0f + (18.0f * (float) i), 60.0f, 18.0f, car_image_bmp, true);
      this->totals_car_images.push_back(image);
      this->ui_elements.push_back(image);
    } else {
      this->totals_car_images[i]->setImage(car_image_bmp);
    }
  }
}

Action HallOfFame::get_exit_action(Menu last_menu)
{
  switch (last_menu) {
  case Menu::SUNDAY_CUP:
    return Action::OPEN_SUNDAY_CUP;
    break;
  case Menu::TIME_TRIAL_MENU:
    return Action::OPEN_TIME_TRIAL_MENU;
    break;
  case Menu::LEAGUE:
    return Action::OPEN_LEAGUE;
    break;
  default:
    return Action::OPEN_RACE_MENU;
    break;
  }
}
