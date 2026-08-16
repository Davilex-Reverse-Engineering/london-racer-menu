#ifndef MENU_TRACK_INFO_HPP
#define MENU_TRACK_INFO_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"
#include "../IniHandler.hpp"

class TrackInfo : public BaseMenu {
public:
    TrackInfo(Menu last_menu, IniHandler * game_ini_handler, IniHandler * static_ini_handler);
    ~TrackInfo();

private:
    Action get_exit_action(Menu last_menu);
};

#endif // MENU_TRACK_INFO_HPP