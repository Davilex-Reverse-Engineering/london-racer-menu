#ifndef MENU_PLAYER_MENU_HPP
#define MENU_PLAYER_MENU_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"

class PlayerMenu : public BaseMenu {
public:
    PlayerMenu();
    ~PlayerMenu();
};

#endif // MENU_PLAYER_MENU_HPP