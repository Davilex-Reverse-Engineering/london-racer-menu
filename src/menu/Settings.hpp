#ifndef MENU_SETTINGS_HPP
#define MENU_SETTINGS_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"

class Settings : public BaseMenu {
public:
    Settings();
    ~Settings();
};

#endif // MENU_SETTINGS_HPP