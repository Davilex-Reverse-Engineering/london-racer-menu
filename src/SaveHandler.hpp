#ifndef SAVE_HANDLER_HPP
#define SAVE_HANDLER_HPP

#include "../vendor/SDL/include/SDL3/SDL.h"

#include <string>
#include <stdint.h>

class SaveHandler {
public:
  SaveHandler();
  ~SaveHandler();

  bool load();
  bool write();

private:
  const std::string file_name = "Current.rcr";
  const std::string player_name = "Player";

  typedef struct OwnedCar {
    uint32_t damage_percentage;
    bool engine_upgraded;
    bool tires_upgraded;
    bool nitrous_upgraded;
    bool brakes_upgraded;
    bool gearbox_upgraded;
    uint8_t league;
    uint8_t car_id;
  } OwnedCar;

  typedef struct Settings {
    uint32_t resolution_id;
    uint32_t sound_fx_volume;
    uint32_t music_volume;
    uint32_t police_volume;
    bool sound_fx_enabled;
    bool music_enabled;
    uint8_t mystery_value1;
    uint8_t mystery_value2;
    bool info_screen_enabled;
    uint8_t mystery_value3;
    bool police_sound_enabled;
    uint32_t keycode_accelerate;
    uint32_t keycode_brake;
    uint32_t keycode_left;
    uint32_t keycode_right;
    uint32_t keycode_boost;
    uint32_t keycode_handbrake;
    uint32_t keycode_horn;
    uint32_t input_method;
    uint32_t steering_sensitivity;
  } Settings;

  typedef struct RaceRecordEntry {
    bool is_player;
    uint32_t position;
    uint32_t time;
    uint8_t league;
    uint8_t car;
    uint8_t name_length;
    std::string name;  // 32 bytes long in the file
  } RaceRecordEntry;

  typedef struct RaceRecords {
    uint32_t racer_count;
    RaceRecordEntry * records;
    uint32_t lap_count;
    uint32_t * player_lap_times;
  } RaceRecords;

  typedef struct LeaguePointsEntry {
    uint32_t points;
    bool is_player;
    uint32_t league;
    uint32_t last_finish_position;
  } LeaguePointsEntry;

  typedef struct LeagueData {
    uint32_t record_count;
    RaceRecords * track_records;
    uint8_t racer_count;
    uint8_t unknown1;
    uint8_t unknown2;
    uint8_t unknown3;
    LeaguePointsEntry * league_points_entries;  // Count is equal to the amount of racers
    uint32_t unknown4;
    uint32_t unknown5;
    uint32_t track_total_count;
    uint8_t unknown6[10];
  } LeagueData;

  typedef struct SaveData {
    uint8_t signature_length;
    char signature[3];
    uint8_t name_length;
    std::string name;  // Takes up 32 bytes in the file
    uint8_t reserved1[3];
    uint32_t money;
    uint8_t reserved2[4];
    uint64_t timestamp;
    uint32_t selected_car;
    uint32_t owned_car_count;
    OwnedCar * owned_cars;
    Settings settings;
    uint32_t current_league;  // Can be 0, 1 or 2
    bool all_leagues_completed;
    bool league1_unlocked;
    bool league2_unlocked;
    bool league_introduction_watched;
    uint32_t league_to_show_introduction_for;  // Only used if league_introduction_watched is false
    uint8_t unknown2[3];  // This seems to always be filled with 0s
    uint8_t races_completed_in_current_league;
    uint8_t unknown3[3];
    bool league_has_race[10];  // List of the same amount of true values at the league has races, wrong too
    LeagueData league0;
    LeagueData league1;
    LeagueData league2;
    uint8_t unknown4[6];
    uint32_t unknown5;
    uint32_t unknown6;
    bool league0_car_unlocked[10];
    bool league1_car_unlocked[10];
    bool league2_car_unlocked[10];
    uint32_t track_to_unlock_count;
    bool * track_unlocked;
    uint32_t track_count;
    uint32_t * best_finish_position;
    uint32_t time_trial_track_count;
    bool * time_trial_beaten;
    RaceRecords last_race_records;
  } SaveData;

  SaveData save_data;

  bool readOwnedCar(SDL_IOStream * stream, OwnedCar * owned_car);
  bool readSettings(SDL_IOStream * stream, Settings * settings);
  bool readRaceRecordEntry(SDL_IOStream * stream, RaceRecordEntry * race_record_entry);
  bool readRaceRecords(SDL_IOStream * stream, RaceRecords * race_records);
  bool readLeaguePointsEntry(SDL_IOStream * stream, LeaguePointsEntry * league_points_entry);
  bool readLeagueData(SDL_IOStream * stream, LeagueData * league_data);

};

#endif // SAVE_HANDLER_HPP