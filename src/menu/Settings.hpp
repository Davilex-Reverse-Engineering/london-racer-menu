#ifndef MENU_SETTINGS_HPP
#define MENU_SETTINGS_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"
#include "../IniHandler.hpp"
#include "../ui/UiCheckbox.hpp"

class Settings : public BaseMenu {
public:
    Settings(IniHandler * game_ini_handler);
    ~Settings();

    Action update(std::vector<Input> inputs);

private:
    IniHandler * game_ini_handler = NULL;

    UiCheckbox * checkbox_music = NULL;
    UiCheckbox * checkbox_sound = NULL;
    UiCheckbox * checkbox_police = NULL;

    UiCheckbox * checkbox_res_1 = NULL;
    UiCheckbox * checkbox_res_2 = NULL;
    UiCheckbox * checkbox_res_3 = NULL;
};

#endif // MENU_SETTINGS_HPP