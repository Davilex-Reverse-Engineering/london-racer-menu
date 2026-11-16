#include "SaveHandler.hpp"

#include <cstring>

#include "Utils.hpp"
#include "constants.hpp"

SaveHandler::SaveHandler()
{
}

SaveHandler::~SaveHandler()
{
  this->unload();
}

bool SaveHandler::load()
{
  this->unload();
  SDL_IOStream * stream = SDL_IOFromFile(Utils::getFullPath(this->file_name).c_str(), "rb");
  if (stream == NULL) {
    SDL_Log("Could not load file Current.rcr as stream");
    return false;
  }

  bool result = this->readSaveData(stream);
  SDL_CloseIO(stream);

  this->loaded = result;
  return result;
}

bool SaveHandler::write()
{
  return false;
}

void SaveHandler::unload()
{
  if (!this->loaded) {
    return;
  }

  if(this->save_data.owned_cars) {
    free(this->save_data.owned_cars);
  }

  this->loaded = false;
}

bool SaveHandler::readSaveData(SDL_IOStream *stream)
{
  if(!this->readFileSignature(stream)) {
    SDL_Log("Failed to validate file signature in save file");
    return false;
  }

  if(!this->readName(stream, &this->save_data.name)) {
    SDL_Log("Failed to read player name in save file");
    return false;
  }

  if(!Utils::validatedReservedBytes(stream, 3)) {
    SDL_Log("Failed to read reserved bytes in save file");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &this->save_data.money)) {
    SDL_Log("Failed to read money in save file");
    return false;
  }

  if(!Utils::validatedReservedBytes(stream, 4)) {
    SDL_Log("Failed to read reserved bytes in save file");
    return false;
  }

  if(!this->readTimestamp(stream)) {
    SDL_Log("Failed to read timestamp in save file");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &this->save_data.selected_car)) {
    SDL_Log("Failed to read selected car in save file");
    return false;
  }

  if(!this->readOwnedCars(stream)) {
    SDL_Log("Failed to read owned cars in save file");
    return false;
  }

  if(!this->readSettings(stream)) {
    SDL_Log("Failed to read settings in save file");
    return false;
  }

  if(!Utils::readLeagueIdU32(stream, &this->save_data.current_league)) {
    SDL_Log("Failed to read current league in save file");
    return false;
  }

  if(!Utils::readBoolean(stream, &this->save_data.all_leagues_completed)) {
    SDL_Log("Failed to read if all leagues have been completed in save file");
    return false;
  }

  if(!Utils::readBoolean(stream, &this->save_data.league1_unlocked)) {
    SDL_Log("Failed to read if league 1 is unlocked in save file");
    return false;
  }

  if(!Utils::readBoolean(stream, &this->save_data.league2_unlocked)) {
    SDL_Log("Failed to read if league 1 is unlocked in save file");
    return false;
  }

  if(!Utils::readBoolean(stream, &this->save_data.league_introduction_watched)) {
    SDL_Log("Failed to read if the league introduction was watched in save file");
    return false;
  }

  if(!Utils::readLeagueIdU32(stream, &this->save_data.league_to_show_introduction_for)) {
    SDL_Log("Failed to read league to show introduction for in save file");
    return false;
  }

  if(!SDL_ReadIO(stream, this->save_data.unknown1, 3)) {
    SDL_Log("Failed to read unknown1 in save file");
    return false;
  }

  if(!SDL_ReadU8(stream, &this->save_data.races_completed_in_current_league)) {
    SDL_Log("Failed to read races completed in current league in save file");
    return false;
  }

  if(!SDL_ReadIO(stream, this->save_data.unknown2, 3)) {
    SDL_Log("Failed to read unknown2 in save file");
    return false;
  }

  if(!SDL_ReadIO(stream, this->save_data.league_has_race, 10)) {
    SDL_Log("Failed to read league has race list in save file");
    return false;
  }

  if(!this->readLeagueData(stream, &this->save_data.league0)) {
    SDL_Log("Failed to read league 0 data in save file");
    return false;
  }

  if(!this->readLeagueData(stream, &this->save_data.league1)) {
    SDL_Log("Failed to read league 1 data in save file");
    return false;
  }

  if(!this->readLeagueData(stream, &this->save_data.league2)) {
    SDL_Log("Failed to read league 2 data in save file");
    return false;
  }

  if(!SDL_ReadIO(stream, this->save_data.unknown3, 6)) {
    SDL_Log("Failed to read unknown3 in save file");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &this->save_data.unknown4)) {
    SDL_Log("Failed to read unknown4 in save file");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &this->save_data.unknown5)) {
    SDL_Log("Failed to read unknown5 in save file");
    return false;
  }

  if(!SDL_ReadIO(stream, this->save_data.league0_car_unlocked, CAR_COUNT)) {
    SDL_Log("Failed to read league 0 unlocked cars in save file");
    return false;
  }

  if(!SDL_ReadIO(stream, this->save_data.league1_car_unlocked, CAR_COUNT)) {
    SDL_Log("Failed to read league 1 unlocked cars in save file");
    return false;
  }

  if(!SDL_ReadIO(stream, this->save_data.league2_car_unlocked, CAR_COUNT)) {
    SDL_Log("Failed to read league 2 unlocked cars in save file");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &this->save_data.track_to_unlock_count)) {
    SDL_Log("Failed to read track to unlock count in save file");
    return false;
  }
  if(this->save_data.track_to_unlock_count > 0) {
    this->save_data.track_unlocked = (bool *) calloc(this->save_data.track_to_unlock_count, sizeof(bool));
    for(size_t i = 0; i < this->save_data.track_to_unlock_count; i++) {
      if(!Utils::readBoolean(stream, &this->save_data.track_unlocked[i])) {
        SDL_Log("Failed to load if track %lu was unlocked from save data", i);
        free(this->save_data.track_unlocked);
        this->save_data.track_unlocked = nullptr;
        return false;
      }
    }
  } else {
    SDL_Log("For some reason there is no track unlock information in the save data");
    this->save_data.track_unlocked = nullptr;
  }

  if(!SDL_ReadU32LE(stream, &this->save_data.track_count)) {
    SDL_Log("Failed to read track count in save file");
    if(this->save_data.track_unlocked) {
      free(this->save_data.track_unlocked);
      this->save_data.track_unlocked = nullptr;
    }
    return false;
  }
  if(this->save_data.track_count > 0) {
    this->save_data.best_finish_position = (uint32_t *) calloc(this->save_data.track_to_unlock_count, sizeof(uint32_t));
    for(size_t i = 0; i < this->save_data.track_count; i++) {
      if(!SDL_ReadU32LE(stream, &this->save_data.best_finish_position[i])) {
        SDL_Log("Failed to load best finish position for track %lu from save data", i);
        if(this->save_data.track_unlocked) {
          free(this->save_data.track_unlocked);
          this->save_data.track_unlocked = nullptr;
        }
        free(this->save_data.best_finish_position);
        this->save_data.best_finish_position = nullptr;
        return false;
      }
    }
  } else {
    SDL_Log("For some reason there are no track finish positions in the save data");
    this->save_data.best_finish_position = nullptr;
  }

  if(!SDL_ReadU32LE(stream, &this->save_data.time_trial_track_count)) {
    SDL_Log("Failed to read track count in save file");
    if(this->save_data.track_unlocked) {
      free(this->save_data.track_unlocked);
      this->save_data.track_unlocked = nullptr;
    }
    if(this->save_data.best_finish_position) {
      free(this->save_data.best_finish_position);
      this->save_data.best_finish_position = nullptr;
    }
    return false;
  }
  if(this->save_data.time_trial_track_count > 0) {
    this->save_data.time_trial_beaten = (bool *) calloc(this->save_data.time_trial_track_count, sizeof(bool));
    for(size_t i = 0; i < this->save_data.time_trial_track_count; i++) {
      if(!Utils::readBoolean(stream, &this->save_data.time_trial_beaten[i])) {
        SDL_Log("Failed to load if track %lu was beaten in time trial from save data", i);
        if(this->save_data.track_unlocked) {
          free(this->save_data.track_unlocked);
          this->save_data.track_unlocked = nullptr;
        }
        if(this->save_data.best_finish_position) {
          free(this->save_data.best_finish_position);
          this->save_data.best_finish_position = nullptr;
        }
        free(this->save_data.time_trial_beaten);
        this->save_data.time_trial_beaten = nullptr;
        return false;
      }
    }
  } else {
    SDL_Log("For some reason there are no time trials beaten information in the save data");
    this->save_data.time_trial_beaten = nullptr;
  }

  if(!this->readRaceRecords(stream, &this->save_data.last_race_records)) {
    SDL_Log("Failed to load records from last race from save data");
    if(this->save_data.track_unlocked) {
      free(this->save_data.track_unlocked);
      this->save_data.track_unlocked = nullptr;
    }
    if(this->save_data.best_finish_position) {
      free(this->save_data.best_finish_position);
      this->save_data.best_finish_position = nullptr;
    }
    if(this->save_data.time_trial_beaten) {
      free(this->save_data.time_trial_beaten);
      this->save_data.time_trial_beaten = nullptr;
    }
    return false;
  }

  return true;
}

bool SaveHandler::writeSaveData(SDL_IOStream *stream)
{
  if(!this->writeFileSignature(stream)) {
    SDL_Log("Failed to write file signature in save file");
    return false;
  }

  if(!this->writeName(stream, this->save_data.name)) {
    SDL_Log("Failed to write player name in save file");
    return false;
  }

  if(!Utils::writeReservedBytes(stream, 3)) {
    SDL_Log("Failed to write reserved bytes in save file");
    return false;
  }

  if(!SDL_WriteU32LE(stream, this->save_data.money)) {
    SDL_Log("Failed to write money in save file");
    return false;
  }

  if(!Utils::writeReservedBytes(stream, 4)) {
    SDL_Log("Failed to write reserved bytes in save file");
    return false;
  }

  if(!this->writeTimestamp(stream)) {
    SDL_Log("Failed to write timestamp in save file");
    return false;
  }

  if(!SDL_WriteU32LE(stream, this->save_data.selected_car)) {
    SDL_Log("Failed to write selected car in save file");
    return false;
  }

  if(!this->writeOwnedCars(stream)) {
    SDL_Log("Failed to write owned cars in save file");
    return false;
  }

  if(!this->writeSettings(stream)) {
    SDL_Log("Failed to write settings in save file");
    return false;
  }

  if(!SDL_WriteU32LE(stream, this->save_data.current_league)) {
    SDL_Log("Failed to write current league in save file");
    return false;
  }

  if(!SDL_WriteU8(stream, (uint8_t) this->save_data.all_leagues_completed)) {
    SDL_Log("Failed to write if all leagues have been completed in save file");
    return false;
  }

  if(!SDL_WriteU8(stream, (uint8_t) this->save_data.league1_unlocked)) {
    SDL_Log("Failed to write if league 1 is unlocked in save file");
    return false;
  }

  if(!SDL_WriteU8(stream, (uint8_t) this->save_data.league2_unlocked)) {
    SDL_Log("Failed to write if league 2 is unlocked in save file");
    return false;
  }

  if(!SDL_WriteU8(stream, (uint8_t) this->save_data.league_introduction_watched)) {
    SDL_Log("Failed to write if the league introduction was watched in save file");
    return false;
  }

  if(!SDL_WriteU32LE(stream, this->save_data.league_to_show_introduction_for)) {
    SDL_Log("Failed to write league to show introduction for in save file");
    return false;
  }

  if(!SDL_WriteIO(stream, this->save_data.unknown1, 3)) {
    SDL_Log("Failed to write unknown1 in save file");
    return false;
  }

  if(!SDL_WriteU8(stream, this->save_data.races_completed_in_current_league)) {
    SDL_Log("Failed to write races completed in current league in save file");
    return false;
  }

  if(!SDL_WriteIO(stream, this->save_data.unknown2, 3)) {
    SDL_Log("Failed to write unknown2 in save file");
    return false;
  }

  for(int i = 0; i < 10; i++) {
    if(!SDL_WriteU8(stream, this->save_data.league_has_race[i])) {
      SDL_Log("Failed to write league has race list in save file");
      return false;
    }
  }

  if(!this->writeLeagueData(stream, &this->save_data.league0)) {
    SDL_Log("Failed to write league 0 data in save file");
    return false;
  }

  if(!this->writeLeagueData(stream, &this->save_data.league1)) {
    SDL_Log("Failed to write league 1 data in save file");
    return false;
  }

  if(!this->writeLeagueData(stream, &this->save_data.league2)) {
    SDL_Log("Failed to write league 2 data in save file");
    return false;
  }

  if(!SDL_WriteIO(stream, this->save_data.unknown3, 6)) {
    SDL_Log("Failed to write unknown3 in save file");
    return false;
  }

  if(!SDL_WriteU32LE(stream, this->save_data.unknown4)) {
    SDL_Log("Failed to write unknown4 in save file");
    return false;
  }

  if(!SDL_WriteU32LE(stream, this->save_data.unknown5)) {
    SDL_Log("Failed to write unknown5 in save file");
    return false;
  }

  for(int i = 0; i < CAR_COUNT; i++) {
    if(!SDL_WriteU8(stream, this->save_data.league0_car_unlocked[i])) {
      SDL_Log("Failed to write league 0 unlocked cars in save file");
      return false;
    }
  }

  for(int i = 0; i < CAR_COUNT; i++) {
    if(!SDL_WriteU8(stream, this->save_data.league1_car_unlocked[i])) {
      SDL_Log("Failed to write league 1 unlocked cars in save file");
      return false;
    }
  }

  for(int i = 0; i < CAR_COUNT; i++) {
    if(!SDL_WriteU8(stream, this->save_data.league2_car_unlocked[i])) {
      SDL_Log("Failed to write league 2 unlocked cars in save file");
      return false;
    }
  }

  if(!SDL_WriteU32LE(stream, this->save_data.track_to_unlock_count)) {
    SDL_Log("Failed to write track to unlock count in save file");
    return false;
  }

  for(size_t i = 0; i < this->save_data.track_to_unlock_count; i++) {
    if(!SDL_WriteU8(stream, (uint8_t) this->save_data.track_unlocked[i])) {
      SDL_Log("Failed to write if track %lu was unlocked to save data", i);
      return false;
    }
  }

  if(!SDL_WriteU32LE(stream, this->save_data.track_count)) {
    SDL_Log("Failed to write track count in save file");
    return false;
  }

  for(size_t i = 0; i < this->save_data.track_count; i++) {
    if(!SDL_WriteU32LE(stream, this->save_data.best_finish_position[i])) {
      SDL_Log("Failed to write best finish position for track %lu to save data", i);
      return false;
    }
  }

  if(!SDL_WriteU32LE(stream, this->save_data.time_trial_track_count)) {
    SDL_Log("Failed to write time trial track count in save file");
    return false;
  }

  for(size_t i = 0; i < this->save_data.time_trial_track_count; i++) {
    if(!SDL_WriteU8(stream, (uint8_t) this->save_data.time_trial_beaten[i])) {
      SDL_Log("Failed to write if track %lu was beaten in time trial to save data", i);
      return false;
    }
  }

  if(!this->writeRaceRecords(stream, &this->save_data.last_race_records)) {
    SDL_Log("Failed to write records from last race to save data");
    return false;
  }

  return true;
}

bool SaveHandler::readOwnedCars(SDL_IOStream *stream)
{
  if(!SDL_ReadU32LE(stream, &this->save_data.owned_car_count)) {
    SDL_Log("Failed to owned car count");
    return false;
  }

  // Do not run calloc if there are no owned cars
  if(this->save_data.owned_car_count == 0) {
    this->save_data.owned_cars = nullptr;
    return true;
  }

  if(this->save_data.selected_car >= this->save_data.owned_car_count) {
    SDL_Log("Selected car is outside of owned car count, failing");
    this->save_data.owned_cars = nullptr;
    return false;
  }

  this->save_data.owned_cars = (OwnedCar *) calloc(this->save_data.owned_car_count, sizeof(OwnedCar));
  for(size_t i = 0; i < this->save_data.owned_car_count; i++) {
    OwnedCar * car = &this->save_data.owned_cars[i];
    if(!SDL_ReadU32LE(stream, &car->damage_percentage)) {
      SDL_Log("Failed to get damage percentage count");
      free(this->save_data.owned_cars);
      this->save_data.owned_cars = nullptr;
      return false;
    }
    if(car->damage_percentage > 100) {
      SDL_Log("Damage percentage was invalid value %u", car->damage_percentage);
      free(this->save_data.owned_cars);
      this->save_data.owned_cars = nullptr;
      return false;
    }
    
    if(!Utils::readBoolean(stream, &car->engine_upgraded)) {
      SDL_Log("Failed to get if the engine was upgraded");
      free(this->save_data.owned_cars);
      this->save_data.owned_cars = nullptr;
      return false;
    }
  
    if(!Utils::readBoolean(stream, &car->tires_upgraded)) {
      SDL_Log("Failed to get if the tires were upgraded");
      free(this->save_data.owned_cars);
      this->save_data.owned_cars = nullptr;
      return false;
    }

    if(!Utils::readBoolean(stream, &car->nitrous_upgraded)) {
      SDL_Log("Failed to get if the nitrous was installed");
      free(this->save_data.owned_cars);
      this->save_data.owned_cars = nullptr;
      return false;
    }

    if(!Utils::readBoolean(stream, &car->brakes_upgraded)) {
      SDL_Log("Failed to get if the brakes were upgraded");
      free(this->save_data.owned_cars);
      this->save_data.owned_cars = nullptr;
      return false;
    }

    if(!Utils::readBoolean(stream, &car->gearbox_upgraded)) {
      SDL_Log("Failed to get if the gearbox was upgraded");
      free(this->save_data.owned_cars);
      this->save_data.owned_cars = nullptr;
      return false;
    }

    if(!Utils::readLeagueIdU8(stream, &car->league)) {
      SDL_Log("Failed to get league of car");
      free(this->save_data.owned_cars);
      this->save_data.owned_cars = nullptr;
      return false;
    }

    if(!SDL_ReadU8(stream, &car->car_id)) {
      SDL_Log("Failed to get id of car");
      free(this->save_data.owned_cars);
      this->save_data.owned_cars = nullptr;
      return false;
    }
    if(car->car_id >= CAR_COUNT) {
      SDL_Log("Car id %u is invalid", car->car_id);
      free(this->save_data.owned_cars);
      this->save_data.owned_cars = nullptr;
      return false;
    }
  }

  return true;
}

bool SaveHandler::writeOwnedCars(SDL_IOStream *stream)
{
  if(!SDL_WriteU32LE(stream, this->save_data.owned_car_count)) {
    SDL_Log("Failed to write owned car count");
    return false;
  }

  if(this->save_data.selected_car >= this->save_data.owned_car_count) {
    SDL_Log("Selected car is outside of owned car count, failing");
    return false;
  }

  for(size_t i = 0; i < this->save_data.owned_car_count; i++) {
    OwnedCar * car = &this->save_data.owned_cars[i];
    if(!SDL_WriteU32LE(stream, car->damage_percentage)) {
      SDL_Log("Failed to write damage percentage count");
      return false;
    }

    if(!SDL_WriteU8(stream, (uint8_t) car->engine_upgraded)) {
      SDL_Log("Failed to write if the engine was upgraded");
      return false;
    }

    if(!SDL_WriteU8(stream, (uint8_t) car->tires_upgraded)) {
      SDL_Log("Failed to write if the tires were upgraded");
      return false;
    }

    if(!SDL_WriteU8(stream, (uint8_t) car->nitrous_upgraded)) {
      SDL_Log("Failed to write if the nitrous was installed");
      return false;
    }

    if(!SDL_WriteU8(stream, (uint8_t) car->brakes_upgraded)) {
      SDL_Log("Failed to write if the brakes were upgraded");
      return false;
    }

    if(!SDL_WriteU8(stream, (uint8_t) car->gearbox_upgraded)) {
      SDL_Log("Failed to write if the gearbox was upgraded");
      return false;
    }

    if(!SDL_WriteU8(stream, car->league)) {
      SDL_Log("Failed to write league of car");
      return false;
    }

    if(!SDL_WriteU8(stream, car->car_id)) {
      SDL_Log("Failed to write id of car");
      return false;
    }
  }

  return true;
}

bool SaveHandler::readSettings(SDL_IOStream *stream)
{
  Settings * settings = &this->save_data.settings;
  if(!SDL_ReadU32LE(stream, &settings->resolution_id)) {
    SDL_Log("Failed to get resolution id from settings");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &settings->sound_fx_volume)) {
    SDL_Log("Failed to get sound fx volume from settings");
    return false;
  }
  if(settings->sound_fx_volume > 100) {
    SDL_Log("Sound fx volume was invalid value %u", settings->sound_fx_volume);
    return false;
  }

  if(!SDL_ReadU32LE(stream, &settings->music_volume)) {
    SDL_Log("Failed to get music volume from settings");
    return false;
  }
  if(settings->music_volume > 100) {
    SDL_Log("Music volume was invalid value %u", settings->music_volume);
    return false;
  }

  if(!SDL_ReadU32LE(stream, &settings->police_volume)) {
    SDL_Log("Failed to get police volume from settings");
    return false;
  }
  if(settings->police_volume > 100) {
    SDL_Log("Police volume was invalid value %u", settings->police_volume);
    return false;
  }

  if(!Utils::readBoolean(stream, &settings->sound_fx_enabled)) {
    SDL_Log("Failed to get if sound effects are enabled from settings");
    return false;
  }

  if(!Utils::readBoolean(stream, &settings->music_enabled)) {
    SDL_Log("Failed to get if music is enabled from settings");
    return false;
  }

  if(!SDL_ReadU8(stream, &settings->mystery_value1)) {
    SDL_Log("Failed to get mystery value 1 from settings");
    return false;
  }

  if(!SDL_ReadU8(stream, &settings->mystery_value2)) {
    SDL_Log("Failed to get mystery value 2 from settings");
    return false;
  }

  if(!Utils::readBoolean(stream, &settings->info_screen_enabled)) {
    SDL_Log("Failed to get if info screen is enabled from settings");
    return false;
  }

  if(!SDL_ReadU8(stream, &settings->mystery_value3)) {
    SDL_Log("Failed to get mystery value 3 from settings");
    return false;
  }

  if(!Utils::readBoolean(stream, &settings->police_sound_enabled)) {
    SDL_Log("Failed to get if police sound is enabled from settings");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &settings->keycode_accelerate)) {
    SDL_Log("Failed to get accelerate keycode from settings");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &settings->keycode_brake)) {
    SDL_Log("Failed to get brake keycode from settings");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &settings->keycode_left)) {
    SDL_Log("Failed to get left keycode from settings");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &settings->keycode_right)) {
    SDL_Log("Failed to get right keycode from settings");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &settings->keycode_boost)) {
    SDL_Log("Failed to get boost keycode from settings");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &settings->keycode_handbrake)) {
    SDL_Log("Failed to get handbrake keycode from settings");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &settings->keycode_horn)) {
    SDL_Log("Failed to get horn keycode from settings");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &settings->input_method)) {
    SDL_Log("Failed to get input method from settings");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &settings->steering_sensitivity)) {
    SDL_Log("Failed to get steering sensitivity from settings");
    return false;
  }
  if(settings->steering_sensitivity > 100) {
    SDL_Log("steering sensitivity was invalid value %u", settings->steering_sensitivity);
    return false;
  }

  return true;
}

bool SaveHandler::writeSettings(SDL_IOStream *stream)
{
  Settings * settings = &this->save_data.settings;
  if(!SDL_WriteU32LE(stream, settings->resolution_id)) {
    SDL_Log("Failed to write resolution id to settings");
    return false;
  }

  if(!SDL_WriteU32LE(stream, settings->sound_fx_volume)) {
    SDL_Log("Failed to write sound fx volume to settings");
    return false;
  }

  if(!SDL_WriteU32LE(stream, settings->music_volume)) {
    SDL_Log("Failed to write music volume to settings");
    return false;
  }

  if(!SDL_WriteU32LE(stream, settings->police_volume)) {
    SDL_Log("Failed to write police volume to settings");
    return false;
  }

  if(!SDL_WriteU8(stream, (uint8_t) settings->sound_fx_enabled)) {
    SDL_Log("Failed to write if sound effects are enabled from settings");
    return false;
  }

  if(!SDL_WriteU8(stream, (uint8_t) settings->music_enabled)) {
    SDL_Log("Failed to write if music is enabled from settings");
    return false;
  }

  if(!SDL_WriteU8(stream, settings->mystery_value1)) {
    SDL_Log("Failed to write mystery value 1 to settings");
    return false;
  }

  if(!SDL_WriteU8(stream, settings->mystery_value2)) {
    SDL_Log("Failed to write mystery value 2 to settings");
    return false;
  }

  if(!SDL_WriteU8(stream, (uint8_t) settings->info_screen_enabled)) {
    SDL_Log("Failed to write if info screen is enabled to settings");
    return false;
  }

  if(!SDL_WriteU8(stream, settings->mystery_value3)) {
    SDL_Log("Failed to write mystery value 3 to settings");
    return false;
  }

  if(!SDL_WriteU8(stream, (uint8_t) settings->police_sound_enabled)) {
    SDL_Log("Failed to write if police sound is enabled to settings");
    return false;
  }

  if(!SDL_WriteU32LE(stream, (uint32_t) settings->keycode_accelerate)) {
    SDL_Log("Failed to write accelerate keycode to settings");
    return false;
  }

  if(!SDL_WriteU32LE(stream, (uint32_t) settings->keycode_brake)) {
    SDL_Log("Failed to write brake keycode to settings");
    return false;
  }

  if(!SDL_WriteU32LE(stream, (uint32_t) settings->keycode_left)) {
    SDL_Log("Failed to write left keycode to settings");
    return false;
  }

  if(!SDL_WriteU32LE(stream, (uint32_t) settings->keycode_right)) {
    SDL_Log("Failed to write right keycode to settings");
    return false;
  }

  if(!SDL_WriteU32LE(stream, (uint32_t) settings->keycode_boost)) {
    SDL_Log("Failed to write boost keycode to settings");
    return false;
  }

  if(!SDL_WriteU32LE(stream, (uint32_t) settings->keycode_handbrake)) {
    SDL_Log("Failed to write handbrake keycode to settings");
    return false;
  }

  if(!SDL_WriteU32LE(stream, (uint32_t) settings->keycode_horn)) {
    SDL_Log("Failed to write horn keycode to settings");
    return false;
  }

  if(!SDL_WriteU32LE(stream, (uint32_t) settings->input_method)) {
    SDL_Log("Failed to write input method to settings");
    return false;
  }

  if(!SDL_WriteU32LE(stream, settings->steering_sensitivity)) {
    SDL_Log("Failed to write steering sensitivity to settings");
    return false;
  }

  return true;
}

bool SaveHandler::readRaceRecordEntry(SDL_IOStream *stream, RaceRecordEntry *race_record_entry)
{
  if(!Utils::readBoolean(stream, &race_record_entry->is_player)) {
    SDL_Log("Failed to get if record entry is for a player");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &race_record_entry->position)) {
    SDL_Log("Failed to get position from record entry");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &race_record_entry->time)) {
    SDL_Log("Failed to get time from record entry");
    return false;
  }

  if(!Utils::readLeagueIdU8(stream, &race_record_entry->league)) {
    SDL_Log("Failed to get league from record entry");
    return false;
  }

  if(!SDL_ReadU8(stream, &race_record_entry->car)) {
    SDL_Log("Failed to get car from record entry");
    return false;
  }
  if (race_record_entry->car >= CAR_COUNT) {
    SDL_Log("Car id %u found in record entry is invalid, failing", race_record_entry->car);
    return false;
  }

  if(!this->readName(stream, &race_record_entry->name)) {
    SDL_Log("Failed to get name from record entry");
    return false;
  }

  return true;
}

bool SaveHandler::writeRaceRecordEntry(SDL_IOStream *stream, RaceRecordEntry *race_record_entry)
{
  if(!SDL_WriteU8(stream, (uint8_t) race_record_entry->is_player)) {
    SDL_Log("Failed to get if record entry is for a player");
    return false;
  }

  if(!SDL_WriteU32LE(stream, race_record_entry->position)) {
    SDL_Log("Failed to write position to record entry");
    return false;
  }

  if(!SDL_WriteU32LE(stream, race_record_entry->time)) {
    SDL_Log("Failed to write time to record entry");
    return false;
  }

  if(!SDL_WriteU8(stream, race_record_entry->league)) {
    SDL_Log("Failed to write league to record entry");
    return false;
  }

  if(!SDL_WriteU8(stream, race_record_entry->car)) {
    SDL_Log("Failed to write car to record entry");
    return false;
  }

  if(!this->writeName(stream, race_record_entry->name)) {
    SDL_Log("Failed to write name to record entry");
    return false;
  }

  return true;
}

bool SaveHandler::readRaceRecords(SDL_IOStream *stream, RaceRecords *race_records)
{
  if(!SDL_ReadU32LE(stream, &race_records->racer_count)) {
    SDL_Log("Failed to get count from record data");
    return false;
  }
  if(race_records->racer_count > 0) {
    race_records->records = (RaceRecordEntry *) calloc(race_records->racer_count, sizeof(RaceRecordEntry));
    for(size_t i = 0; i < race_records->racer_count; i++) {
      if(!this->readRaceRecordEntry(stream, &race_records->records[i])) {
        SDL_Log("Failed to get entry from record data");
        free(race_records->records);
        race_records->records = nullptr;
        return false;
      }
    }
  } else {
    race_records->records = nullptr;
  }

  if(!SDL_ReadU32LE(stream, &race_records->lap_count)) {
    SDL_Log("Failed to get lap count from record data");
    return false;
  }
  if(race_records->lap_count > 0) {
    race_records->player_lap_times = (uint32_t *) calloc(race_records->lap_count, sizeof(uint32_t));
    for(size_t i = 0; i < race_records->lap_count; i++) {
      if(!SDL_ReadU32LE(stream, &race_records->player_lap_times[i])) {
        SDL_Log("Failed to get player lap time from record data");
        free(race_records->records);
        race_records->records = nullptr;
        free(race_records->player_lap_times);
        race_records->player_lap_times = nullptr;
        return false;
      }
    }
  } else {
    race_records->player_lap_times = nullptr;
  }

  return true;
}

bool SaveHandler::writeRaceRecords(SDL_IOStream *stream, RaceRecords *race_records)
{
  if(!SDL_WriteU32LE(stream, race_records->racer_count)) {
    SDL_Log("Failed to write count to record data");
    return false;
  }
  for(size_t i = 0; i < race_records->racer_count; i++) {
    if(!this->writeRaceRecordEntry(stream, &race_records->records[i])) {
      SDL_Log("Failed to write entry to record data");
      return false;
    }
  }

  if(!SDL_WriteU32LE(stream, race_records->lap_count)) {
    SDL_Log("Failed to write lap count to record data");
    return false;
  }

  for(size_t i = 0; i < race_records->lap_count; i++) {
    if(!SDL_WriteU32LE(stream, race_records->player_lap_times[i])) {
      SDL_Log("Failed to write player lap time to record data");
      return false;
    }
  }

  return true;
}

bool SaveHandler::readLeaguePointsEntry(SDL_IOStream *stream, LeaguePointsEntry *league_points_entry)
{
  if(!SDL_ReadU32LE(stream, &league_points_entry->points)) {
    SDL_Log("Failed to get points from league points entry");
    return false;
  }

  if(!Utils::readBoolean(stream, &league_points_entry->is_player)) {
    SDL_Log("Failed to get is player from league points entry");
    return false;
  }

  if(!Utils::readLeagueIdU32(stream, &league_points_entry->league)) {
    SDL_Log("Failed to get league from league points entry");
    return false;
  }

  if(!SDL_ReadU32LE(stream, &league_points_entry->last_finish_position)) {
    SDL_Log("Failed to get last finish position from league points entry");
    return false;
  }

  return true;
}

bool SaveHandler::writeLeaguePointsEntry(SDL_IOStream *stream, LeaguePointsEntry *league_points_entry)
{
  if(!SDL_WriteU32LE(stream, league_points_entry->points)) {
    SDL_Log("Failed to write points to league points entry");
    return false;
  }

  if(!SDL_WriteU8(stream, (uint8_t) league_points_entry->is_player)) {
    SDL_Log("Failed to write is player to league points entry");
    return false;
  }

  if(!SDL_WriteU32LE(stream, league_points_entry->league)) {
    SDL_Log("Failed to write league to league points entry");
    return false;
  }

  if(!SDL_WriteU32LE(stream, league_points_entry->last_finish_position)) {
    SDL_Log("Failed to write last finish position to league points entry");
    return false;
  }

  return true;
}

bool SaveHandler::readLeagueData(SDL_IOStream *stream, LeagueData *league_data)
{
  if(!SDL_ReadU32LE(stream, &league_data->record_count)) {
    SDL_Log("Failed to get record count from league data");
    return false;
  }
  if(league_data->record_count > 0) {
    league_data->race_records = (RaceRecords *) calloc(league_data->record_count, sizeof(RaceRecords));
    for(size_t i = 0; i < league_data->record_count; i++) {
      if(!this->readRaceRecords(stream, &league_data->race_records[i])) {
        SDL_Log("Failed to get record from league data");
        free(league_data->race_records);
        league_data->race_records = nullptr;
        return false;
      }
    }
  } else {
    league_data->race_records = nullptr;
  }

  if(!SDL_ReadU8(stream, &league_data->racer_count)) {
    SDL_Log("Failed to get racer count from league data");
    if (league_data->race_records) {
      free(league_data->race_records);
      league_data->race_records = nullptr;
    }
    return false;
  }

  if(!SDL_ReadU8(stream, &league_data->unknown1)) {
    SDL_Log("Failed to get unknown1 from league data");
    if (league_data->race_records) {
      free(league_data->race_records);
      league_data->race_records = nullptr;
    }
    return false;
  }

  if(!SDL_ReadU8(stream, &league_data->unknown2)) {
    SDL_Log("Failed to get unknown2 from league data");
    if (league_data->race_records) {
      free(league_data->race_records);
      league_data->race_records = nullptr;
    }
    return false;
  }

  if(!SDL_ReadU8(stream, &league_data->unknown3)) {
    SDL_Log("Failed to get unknown3 from league data");
    if (league_data->race_records) {
      free(league_data->race_records);
      league_data->race_records = nullptr;
    }
    return false;
  }

  if(league_data->racer_count > 0) {
    league_data->league_points_entries = (LeaguePointsEntry *) calloc(league_data->racer_count, sizeof(LeaguePointsEntry));
    for(size_t i = 0; i < league_data->racer_count; i++) {
      if(!this->readLeaguePointsEntry(stream, &league_data->league_points_entries[i])) {
        SDL_Log("Failed to get league points entry from league data");
        if (league_data->race_records) {
          free(league_data->race_records);
          league_data->race_records = nullptr;
        }
        free(league_data->league_points_entries);
        league_data->league_points_entries = nullptr;
        return false;
      }
    }
  }

  if(!SDL_ReadU32LE(stream, &league_data->unknown4)) {
    SDL_Log("Failed to get unknown4 from league data");
    if (league_data->race_records) {
      free(league_data->race_records);
      league_data->race_records = nullptr;
    }
    if (league_data->league_points_entries) {
      free(league_data->league_points_entries);
      league_data->league_points_entries = nullptr;
    }
    return false;
  }

  if(!SDL_ReadU32LE(stream, &league_data->unknown5)) {
    SDL_Log("Failed to get unknown3 from league data");
    if (league_data->race_records) {
      free(league_data->race_records);
      league_data->race_records = nullptr;
    }
    if (league_data->league_points_entries) {
      free(league_data->league_points_entries);
      league_data->league_points_entries = nullptr;
    }
    return false;
  }

  if(!SDL_ReadU32LE(stream, &league_data->track_total_count)) {
    SDL_Log("Failed to get track total count from league data");
    if (league_data->race_records) {
      free(league_data->race_records);
      league_data->race_records = nullptr;
    }
    if (league_data->league_points_entries) {
      free(league_data->league_points_entries);
      league_data->league_points_entries = nullptr;
    }
    return false;
  }

  if(!SDL_ReadIO(stream, &league_data->unknown6, 10)) {
    SDL_Log("Failed to get unknown6 from league data");
    if (league_data->race_records) {
      free(league_data->race_records);
      league_data->race_records = nullptr;
    }
    if (league_data->league_points_entries) {
      free(league_data->league_points_entries);
      league_data->league_points_entries = nullptr;
    }
    return false;
  }

  return true;
}

bool SaveHandler::writeLeagueData(SDL_IOStream *stream, LeagueData *league_data)
{
  if(!SDL_WriteU32LE(stream, league_data->record_count)) {
    SDL_Log("Failed to write record count to league data");
    return false;
  }

  for(size_t i = 0; i < league_data->record_count; i++) {
    if(!this->writeRaceRecords(stream, &league_data->race_records[i])) {
      SDL_Log("Failed to write record to league data");
      return false;
    }
  }

  if(!SDL_WriteU8(stream, league_data->racer_count)) {
    SDL_Log("Failed to write racer count to league data");
    return false;
  }

  if(!SDL_WriteU8(stream, league_data->unknown1)) {
    SDL_Log("Failed to write unknown1 to league data");
    return false;
  }

  if(!SDL_WriteU8(stream, league_data->unknown2)) {
    SDL_Log("Failed to write unknown2 to league data");
    return false;
  }

  if(!SDL_WriteU8(stream, league_data->unknown3)) {
    SDL_Log("Failed to get unknown3 from league data");
    return false;
  }

  for(size_t i = 0; i < league_data->racer_count; i++) {
    if(!this->writeLeaguePointsEntry(stream, &league_data->league_points_entries[i])) {
      SDL_Log("Failed to write league points entry to league data");
      return false;
    }
  }

  if(!SDL_WriteU32LE(stream, league_data->unknown4)) {
    SDL_Log("Failed to write unknown4 to league data");
    return false;
  }

  if(!SDL_WriteU32LE(stream, league_data->unknown5)) {
    SDL_Log("Failed to write unknown5 to league data");
    return false;
  }

  if(!SDL_WriteU32LE(stream, league_data->track_total_count)) {
    SDL_Log("Failed to write track total count to league data");
    return false;
  }

  if(!SDL_WriteIO(stream, &league_data->unknown6, 10)) {
    SDL_Log("Failed to write unknown6 to league data");
    return false;
  }

  return true;
}

bool SaveHandler::readFileSignature(SDL_IOStream *stream)
{
  if(SDL_TellIO(stream) != 0) {
    SDL_Log("The file signature can only be read from the beginning of the file");
    return false;
  }
  uint8_t signature_length = 0;
  if(!SDL_ReadU8(stream, &signature_length)) {
    SDL_Log("Could not read signature length: %s", SDL_GetError());
    return false;
  }
  if(signature_length != 3) {
    SDL_Log("Signature length was %u instead of the expected 3", signature_length);
    return false;
  }

  uint8_t signature[3];
  if(!SDL_ReadIO(stream, (void *) signature, 3)) {
    SDL_Log("Could not read signature: %s", SDL_GetError());
    return false;
  }
  if(signature[0] != 'R' || signature[1] != 'C' || signature[2] != 'R' ) {
    SDL_Log("Signature does not match expected value RCR");
    return false;
  }

  return true;
}

bool SaveHandler::writeFileSignature(SDL_IOStream *stream)
{
  if(SDL_TellIO(stream) != 0) {
    SDL_Log("The file signature can only be written from the beginning of the file");
    return false;
  }

  uint8_t signature[4];
  signature[0] = 3;
  signature[1] = 'R';
  signature[2] = 'C';
  signature[3] = 'R';
  if(!SDL_WriteIO(stream, (void *) signature, 4)) {
    SDL_Log("Could not write signature: %s", SDL_GetError());
    return false;
  }

  return true;
}

bool SaveHandler::readName(SDL_IOStream *stream, std::string *output_string)
{
  uint8_t length = 0;
  if(!SDL_ReadU8(stream, &length)) {
    SDL_Log("Could not read name length: %s", SDL_GetError());
    return false;
  }
  if(length < 1 || length > 32) {
    SDL_Log("Name length was %u, which is not a valid value", length);
    return false;
  }

  uint8_t name[32];
  if(!SDL_ReadIO(stream, (void *) name, 32)) {
    SDL_Log("Could not read name: %s", SDL_GetError());
    return false;
  }
  *output_string += Utils::Windows1252ToString(name, length);
  if(output_string->empty()) {
    SDL_Log("Name read was empty");
    return false;
  }

  return true;
}

bool SaveHandler::writeName(SDL_IOStream *stream, std::string &input_string)
{
  uint8_t output_length = 0;
  void * output_string = Utils::stringToWindows1252(input_string, &output_length);
  if(!output_string) {
    SDL_Log("Could not convert name to Windows-1252");
    return false;
  }
  if(!SDL_WriteU8(stream, output_length)) {
    SDL_Log("Could not write name length: %s", SDL_GetError());
    return false;
  }
  if(!SDL_WriteIO(stream, output_string, 32)) {
    SDL_Log("Could not write name: %s", SDL_GetError());
    return false;
  }
  return true;
}

bool SaveHandler::readTimestamp(SDL_IOStream *stream) {
  uint64_t data = 0;
  if(!SDL_ReadU64LE(stream, &data)) {
    SDL_Log("Could not timestamp: %s", SDL_GetError());
    return false;
  }
  double ole_timestamp = 0;
  std::memcpy(&ole_timestamp, &data, sizeof(uint64_t));

  // Conversion from OLE automation format to unix timestamp
  // There is a 25569 day difference between unix timestamps and OLE automation format
  SDL_Time timestamp = (ole_timestamp - 25569.0) * 24.0 * 3600.0 * 1e9;
  SDL_TimeToDateTime(timestamp, &this->save_data.timestamp, false);
  SDL_Log("%02d/%02d/%d %02d:%02d:%02d", this->save_data.timestamp.day, this->save_data.timestamp.month, this->save_data.timestamp.year, this->save_data.timestamp.hour, this->save_data.timestamp.minute, this->save_data.timestamp.second);
  return true;
}

bool SaveHandler::writeTimestamp(SDL_IOStream *stream)
{
  SDL_Time unix_timestamp;
  if(!SDL_DateTimeToTime(&this->save_data.timestamp, &unix_timestamp)) {
    SDL_Log("Could not convert date time to unix timestamp: %s", SDL_GetError());
    return false;
  }
  double timestamp = (double) unix_timestamp / 24.0 / 3600.0 / 1e9 + 25569.0;
  uint64_t timestamp_data;
  std::memcpy(&timestamp_data, &timestamp, sizeof(uint64_t));

  if(!SDL_WriteU64LE(stream, timestamp_data)) {
    SDL_Log("Could not write timestamp: %s", SDL_GetError());
    return false;
  }

  return true;
}
