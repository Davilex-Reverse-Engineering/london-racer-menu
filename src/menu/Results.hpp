#ifndef MENU_RESULTS_HPP
#define MENU_RESULTS_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"
#include "../IniHandler.hpp"

class Results : public BaseMenu {
public:
    Results(IniHandler * game_ini_handler, IniHandler * static_ini_handler);
    ~Results();

private:
    IniHandler * game_ini_handler = NULL;
    IniHandler * static_ini_handler = NULL;
};

#endif // MENU_RESULTS_HPP