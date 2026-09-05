#ifndef SCORES_HANDLER_HPP
#define SCORES_HANDLER_HPP

#include "../vendor/SDL/include/SDL3/SDL.h"

#include <string>
#include <map>
#include <vector>

#include "Input.hpp"

typedef struct {
  uint8_t name_length;
  uint8_t name[32];
  int32_t league;
  int32_t car;
  uint32_t time_in_ms;
} ScoreEntry;

typedef struct {
  std::string name;
  int32_t car;
  uint32_t time_in_ms;
} Record;

class ScoresHandler {
public:
  ScoresHandler();
  ~ScoresHandler();

  bool load();

private:
  // these maps look like this: records[track][league][position]
  // Position starts at 0 
  std::map<int32_t, std::map<int32_t, std::map<int32_t, Record>>> lap_records;
  std::map<int32_t, std::map<int32_t, std::map<int32_t, Record>>> total_records;

  std::string name_to_utf8(void * name, size_t length);
};


#endif // SCORES_HANDLER_HPP