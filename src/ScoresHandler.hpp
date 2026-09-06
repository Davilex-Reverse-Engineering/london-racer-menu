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

enum RecordType {
  LAP,
  TOTAL,
};

class ScoresHandler {
public:
  ScoresHandler();
  ~ScoresHandler();

  bool load();
  bool write();
  void printRecords();
  void insertRecord(int32_t track, int32_t league, Record &record, RecordType record_type);

  static std::string getTimeString(uint32_t time_in_ms);

private:
  // these maps look like this: records[track][league][position]
  // Position starts at 0 
  std::map<int32_t, std::map<int32_t, std::vector<Record>>> lap_records;
  std::map<int32_t, std::map<int32_t, std::vector<Record>>> total_records;

  std::string name_to_utf8(void * name, size_t length);
  void * string_to_name(std::string &input_string);

  const std::string file_name = "Scores.dat";
};

#endif // SCORES_HANDLER_HPP