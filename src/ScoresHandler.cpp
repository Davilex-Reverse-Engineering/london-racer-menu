#include "ScoresHandler.hpp"

#include "Utils.hpp"

ScoresHandler::ScoresHandler()
{
}

ScoresHandler::~ScoresHandler()
{
}

bool ScoresHandler::load()
{
  SDL_IOStream * stream = SDL_IOFromFile(Utils::getFullPath(this->file_name).c_str(), "rb");
  if (stream == NULL) {
    SDL_Log("Could not load file Scores.dat as stream");
    return false;
  }

  int32_t track_count = 0;

  // The league count is hard coded here
  for(int32_t league = 0; league < 3; league++) {
    if(!SDL_ReadS32LE(stream, &track_count)) {
      SDL_Log("Could not read track count");
      SDL_CloseIO(stream);
      return false;
    }

    for(int32_t track = 0; track < track_count; track++) {
      if (lap_records.count(track) == 0) {
        lap_records[track] = std::map<int32_t, std::map<int32_t, Record>>();
      }
      if (total_records.count(track) == 0) {
        total_records[track] = std::map<int32_t, std::map<int32_t, Record>>();
      }
      int32_t entry_count = 0;
      for (int32_t record_type = 0; record_type < 2; record_type++) {
        if(!SDL_ReadS32LE(stream, &entry_count)) {
          SDL_Log("Could not read entry count for track %u", track);
          SDL_CloseIO(stream);
          return false;
        }
        for(int32_t i = 0; i < entry_count; i++) {
          if (lap_records[track].count(league) == 0) {
            lap_records[track][league] = std::map<int32_t, Record>();
          }
          if (total_records[track].count(league) == 0) {
            total_records[track][league] = std::map<int32_t, Record>();
          }
          Record entry;
          uint8_t name_length = 0;
          if(!SDL_ReadU8(stream, &name_length) || name_length == 0) {
            SDL_Log("Could not read name length for entry %u for track %u", i, track);
            SDL_CloseIO(stream);
            return false;
          }
          void * name = SDL_calloc(32, sizeof(uint8_t));
          if(!SDL_ReadIO(stream, name, 32)) {
            SDL_Log("Could not read name for entry %u for track %u", i, track);
            SDL_CloseIO(stream);
            return false;
          }
          entry.name = name_to_utf8(name, name_length);
          if (entry.name.empty()) {
            SDL_Log("Could not convert name %s to utf-8", name);
            free(name);
            SDL_CloseIO(stream);
            return false;
          }
          free(name);

          int32_t entry_league = -1;
          if(!SDL_ReadS32LE(stream, &entry_league) || league != entry_league) {
            SDL_Log("Could not read league for entry %u for track %u", i, track);
            SDL_CloseIO(stream);
            return false;
          }
          if(!SDL_ReadS32LE(stream, &entry.car)) {
            SDL_Log("Could not read car for entry %u for track %u", i, track);
            SDL_CloseIO(stream);
            return false;
          }
          if(!SDL_ReadU32LE(stream, &entry.time_in_ms)) {
            SDL_Log("Could not read time for entry %u for track %u", i, track);
            SDL_CloseIO(stream);
            return false;
          }
          if (record_type == 0) {
            if (lap_records[track].count(league) == 0) {
              lap_records[track][league] = std::map<int32_t, Record>();
            }
            lap_records[track][league][i] = entry;
          } else {
            if (total_records[track].count(league) == 0) {
              total_records[track][league] = std::map<int32_t, Record>();
            }
            total_records[track][league][i] = entry;
          }
        }
      }
    }
  }
  if (SDL_GetIOSize(stream) != SDL_SeekIO(stream, 0, SDL_IO_SEEK_CUR)) {
    SDL_Log("Not all bytes of Scores.dat were read!");
  }
  SDL_CloseIO(stream);

  return true;
}

bool ScoresHandler::write()
{
  SDL_IOStream * stream = SDL_IOFromFile(Utils::getFullPath(this->file_name).c_str(), "wb");
  if (stream == NULL) {
    SDL_Log("Could not load file Scores.dat as stream");
    return false;
  }

  for(int32_t league = 0; league < 3; league++) {
    if(!SDL_WriteS32LE(stream, (int32_t) this->lap_records.size())) {
      SDL_Log("Could not write track count");
      SDL_CloseIO(stream);
      return false;
    }
    for(int32_t track = 0; track < (int32_t) this->lap_records.size(); track++) {
      if(!SDL_WriteS32LE(stream, (int32_t) this->lap_records[track][league].size())) {
        SDL_Log("Could not write entry count for track %u", track);
        SDL_CloseIO(stream);
        return false;
      }
      for(int32_t i = 0; i < (int32_t) this->lap_records[track][league].size(); i++) {
        if(!SDL_WriteU8(stream, (uint8_t) this->lap_records[track][league][i].name.size())) {
          SDL_Log("Could not write name length for entry %u for track %u", i, track);
          SDL_CloseIO(stream);
          return false;
        }
        void * name_data = this->string_to_name(this->lap_records[track][league][i].name);
        if(!SDL_WriteIO(stream, name_data, 32)) {
          SDL_Log("Could not write name for entry %u for track %u", i, track);
          SDL_CloseIO(stream);
          return false;
        }
        if(!SDL_WriteS32LE(stream, league)) {
          SDL_Log("Could not write league for entry %u for track %u", i, track);
          SDL_CloseIO(stream);
          return false;
        }
        if(!SDL_WriteS32LE(stream, this->lap_records[track][league][i].car)) {
          SDL_Log("Could not write car for entry %u for track %u", i, track);
          SDL_CloseIO(stream);
          return false;
        }
        if(!SDL_WriteU32LE(stream, this->lap_records[track][league][i].time_in_ms)) {
          SDL_Log("Could not write time for entry %u for track %u", i, track);
          SDL_CloseIO(stream);
          return false;
        }
      }
      if(!SDL_WriteS32LE(stream, (int32_t) this->total_records[track][league].size())) {
        SDL_Log("Could not write entry count for track %u", track);
        SDL_CloseIO(stream);
        return false;
      }
      for(int32_t i = 0; i < (int32_t) this->total_records[track][league].size(); i++) {
        if(!SDL_WriteU8(stream, (uint8_t) this->total_records[track][league][i].name.size())) {
          SDL_Log("Could not write name length for entry %u for track %u", i, track);
          SDL_CloseIO(stream);
          return false;
        }
        void * name_data = this->string_to_name(this->total_records[track][league][i].name);
        if(!SDL_WriteIO(stream, name_data, 32)) {
          SDL_Log("Could not write name for entry %u for track %u", i, track);
          SDL_CloseIO(stream);
          return false;
        }
        if(!SDL_WriteS32LE(stream, league)) {
          SDL_Log("Could not write league for entry %u for track %u", i, track);
          SDL_CloseIO(stream);
          return false;
        }
        if(!SDL_WriteS32LE(stream, this->total_records[track][league][i].car)) {
          SDL_Log("Could not write car for entry %u for track %u", i, track);
          SDL_CloseIO(stream);
          return false;
        }
        if(!SDL_WriteU32LE(stream, this->total_records[track][league][i].time_in_ms)) {
          SDL_Log("Could not write time for entry %u for track %u", i, track);
          SDL_CloseIO(stream);
          return false;
        }
      }
    }
  }

  SDL_CloseIO(stream);
  return false;
}

void ScoresHandler::printRecords()
{
  for (size_t track = 0; track < lap_records.size(); track++) {
    for (size_t league = 0; league < lap_records[track].size(); league++) {
      SDL_Log("\nLap records for track %u league %u:", track, league + 1);
      for (size_t position = 0; position < lap_records[track][league].size(); position++) {
        Record entry = lap_records[track][league][position];
        SDL_Log("%u.  %s with car %u: %s", position + 1, entry.name.c_str(), entry.car, ScoresHandler::getTimeString(entry.time_in_ms).c_str());
      }
      SDL_Log("\nTotal records for track %u league %u:", track, league + 1);
      for (size_t position = 0; position < total_records[track][league].size(); position++) {
        Record entry = total_records[track][league][position];
        SDL_Log("%u.  %s with car %u: %s", position + 1, entry.name.c_str(), entry.car, ScoresHandler::getTimeString(entry.time_in_ms).c_str());
      }
    }
  }
}

std::string ScoresHandler::getTimeString(uint32_t time_in_ms)
{
  uint32_t minutes = time_in_ms / 1000 / 60;
  uint32_t seconds = (time_in_ms % 60000) / 1000;
  uint32_t centiseconds = (time_in_ms % 1000) / 10;

  std::string seconds_string = "";
  if (seconds < 10) {
    seconds_string += "0";
  }
  seconds_string += std::to_string(seconds);


  std::string centiseconds_string = "";
  if (centiseconds < 10) {
    centiseconds_string += "0";
  }
  centiseconds_string += std::to_string(centiseconds);

  return std::to_string(minutes) + ":" + seconds_string + ":" + centiseconds_string;
}

std::string ScoresHandler::name_to_utf8(void * name, size_t length)
{
  SDL_iconv_t iconv = SDL_iconv_open("UTF-8", "ISO-8859-1");
  if ((size_t)iconv == SDL_ICONV_ERROR) {
    SDL_Log("Failed to start iconv, ISO-8859-1 support might not be loadable");
    return "";
  }
  void * original = name;
  size_t inbytesleft = length;
  size_t outbytesleft = length * 3;
  char * target = (char *) SDL_calloc(length, 3);  // 3 is probably overkill tbh
  char * target_start = target;

  size_t iconv_result = SDL_iconv(iconv, (const char **)&original, &inbytesleft, &target, &outbytesleft);
  if (iconv_result == 0) {
    std::string result = std::string((char *)target_start, length);
    free(target_start);
    SDL_iconv_close(iconv);
    return result;
  }

  SDL_Log("Failed convert with status %u (%i)", iconv_result, iconv_result);
  free(target_start);
  SDL_iconv_close(iconv);
  return "";
}

void * ScoresHandler::string_to_name(std::string &input_string)
{
  SDL_iconv_t iconv = SDL_iconv_open("ISO-8859-1", "UTF-8");
  if ((size_t)iconv == SDL_ICONV_ERROR) {
    SDL_Log("Failed to start iconv, ISO-8859-1 support might not be loadable");
    return NULL;
  }

  const char * original = input_string.c_str();
  size_t inbytesleft = input_string.length();
  size_t outbytesleft = 32; // Names are always 32 bytes in total
  void * target = SDL_calloc(outbytesleft, 1);
  void * target_start = target;

  size_t iconv_result = SDL_iconv(iconv, &original, &inbytesleft, (char **)&target, &outbytesleft);
  if (iconv_result == 0) {
    SDL_iconv_close(iconv);
    return target_start;
  }

  SDL_iconv_close(iconv);
  free(target_start);
  return NULL;
}
