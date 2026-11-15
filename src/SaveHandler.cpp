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

  if(!this->readTimestamp(stream, &this->save_data.timestamp)) {
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

bool SaveHandler::readFileSignature(SDL_IOStream * stream)
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

bool SaveHandler::readTimestamp(SDL_IOStream *stream, SDL_DateTime *date_time) {
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
  SDL_TimeToDateTime(timestamp, date_time, false);
  SDL_Log("%02d/%02d/%d %02d:%02d:%02d", date_time->day, date_time->month, date_time->year, date_time->hour, date_time->minute, date_time->second);
  return true;
}
