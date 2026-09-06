#ifndef MENU_RESULTS_HPP
#define MENU_RESULTS_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"
#include "../IniHandler.hpp"

class Results : public BaseMenu {
public:
    Results(IniHandler * game_ini_handler, IniHandler * static_ini_handler, Menu last_menu);
    ~Results();

private:
    IniHandler * game_ini_handler = NULL;
    IniHandler * static_ini_handler = NULL;

    Action get_exit_action(Menu last_menu);
};

#endif // MENU_RESULTS_HPP