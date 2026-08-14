#ifndef PLAYER_MENU_HPP
#define PLAYER_MENU_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"

class PlayerMenu : public BaseMenu {
public:
    PlayerMenu();
    ~PlayerMenu();
};

#endif // PLAYER_MENU_HPP