#ifndef MENU_HALL_OF_FAME_HPP
#define MENU_HALL_OF_FAME_HPP

#include <map>

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"
#include "../IniHandler.hpp"
#include "../ScoresHandler.hpp"
#include "../ui/UiImage.hpp"

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
    std::map<int, std::map<int, UiImage>> car_images;

    void changeTrack(int change);
    void load_track_image();
    void load_records();
    Action get_exit_action(Menu last_menu);
};

#endif // MENU_HALL_OF_FAME_HPP