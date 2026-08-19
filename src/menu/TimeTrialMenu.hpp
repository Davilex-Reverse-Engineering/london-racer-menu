#ifndef MENU_TIME_TRIAL_MENU_HPP
#define MENU_TIME_TRIAL_MENU_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"
#include "../IniHandler.hpp"

class TimeTrialMenu : public BaseMenu {
public:
    TimeTrialMenu(IniHandler * game_ini_handler, IniHandler * static_ini_handler);
    ~TimeTrialMenu();

    Action update(std::vector<Input> inputs);
private:
    IniHandler * game_ini_handler = NULL;
    IniHandler * static_ini_handler = NULL;

};

#endif // MENU_TIME_TRIAL_MENU_HPP