#ifndef TIME_TRIAL_MENU_HPP
#define TIME_TRIAL_MENU_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"

class TimeTrialMenu : public BaseMenu {
public:
    TimeTrialMenu();
    ~TimeTrialMenu();

    Action update(std::vector<Input> inputs);
};

#endif // TIME_TRIAL_MENU_HPP