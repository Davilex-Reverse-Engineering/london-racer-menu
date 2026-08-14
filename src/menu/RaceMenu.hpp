#ifndef RACE_MENU_HPP
#define RACE_MENU_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"

class RaceMenu : public BaseMenu {
public:
    RaceMenu();
    ~RaceMenu();

    Action update(std::vector<Input> inputs);
};

#endif // RACE_MENU_HPP