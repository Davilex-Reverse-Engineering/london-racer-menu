#ifndef MENU_RACE_MENU_HPP
#define MENU_RACE_MENU_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"

class RaceMenu : public BaseMenu {
public:
    RaceMenu();
    ~RaceMenu();

    Action update(std::vector<Input> inputs);
};

#endif // MENU_RACE_MENU_HPP