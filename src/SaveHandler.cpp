#include "SaveHandler.hpp"

#include <cstring>

#include "Utils.hpp"

SaveHandler::SaveHandler()
{
}

SaveHandler::~SaveHandler()
{
}

bool SaveHandler::load()
{
  this->loaded = false;
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
