#ifndef MENU_HALL_OF_FAME_HPP
#define MENU_HALL_OF_FAME_HPP

#include <vector>

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"
#include "../IniHandler.hpp"
#include "../ScoresHandler.hpp"
#include "../ui/UiImage.hpp"
#include "../ui/UiText.hpp"

class HallOfFame : public BaseMenu {
public:
    HallOfFame(IniHandler * game_ini_handler, IniHandler * static_ini_handler, Menu last_menu);
    ~HallOfFame();

    Action update(std::vector<Input> inputs);

private:
    IniHandler * game_ini_handler = NULL;
    IniHandler * static_ini_handler = NULL;
    UiImage * track_image = NULL;

    ScoresHandler scores_handler;

    int track = 0;
    int track_count = 0;

    // League, Car, image
    std::vector<UiImage*> laps_car_images;
    std::vector<UiImage*> totals_car_images;
    std::vector<UiText*> laps_names;
    std::vector<UiText*> totals_names;
    std::vector<UiText*> laps_times;
    std::vector<UiText*> totals_times;

    void changeTrack(int change);
    void load_track_image();
    void load_records();
    Action get_exit_action(Menu last_menu);
    void add_new_record(Menu last_menu);
};

#endif // MENU_HALL_OF_FAME_HPP