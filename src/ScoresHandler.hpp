#ifndef SCORES_HANDLER_HPP
#define SCORES_HANDLER_HPP

#include "../vendor/SDL/include/SDL3/SDL.h"

#include <string>
#include <map>
#include <vector>

#include "Input.hpp"

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
  bool write();
  void printRecords();

  static std::string getTimeString(uint32_t time_in_ms);

private:
  // these maps look like this: records[track][league][position]
  // Position starts at 0 
  std::map<int32_t, std::map<int32_t, std::map<int32_t, Record>>> lap_records;
  std::map<int32_t, std::map<int32_t, std::map<int32_t, Record>>> total_records;

  std::string name_to_utf8(void * name, size_t length);
  void * string_to_name(std::string &input_string);

  const std::string file_name = "Scores.dat";
};


#endif // SCORES_HANDLER_HPP