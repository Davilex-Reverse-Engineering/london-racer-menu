#ifndef MENU_LEAGUE_HPP
#define MENU_LEAGUE_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"
#include "../IniHandler.hpp"

class League : public BaseMenu {
public:
    League(IniHandler * game_ini_handler, IniHandler * static_ini_handler);
    ~League();

    Action update(std::vector<Input> inputs);

private:
    IniHandler * game_ini_handler = NULL;
    IniHandler * static_ini_handler = NULL;
    int number = 1;
};

#endif // MENU_LEAGUE_HPP