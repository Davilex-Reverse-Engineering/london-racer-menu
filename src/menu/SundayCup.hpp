#ifndef MENU_SUNDAY_CUP_HPP
#define MENU_SUNDAY_CUP_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"
#include "../ui/UiText.hpp"
#include "../IniHandler.hpp"

class SundayCup : public BaseMenu {
public:
    SundayCup(IniHandler * game_ini_handler, IniHandler * static_ini_handler);
    ~SundayCup();

    Action update(std::vector<Input> inputs);

private:
    IniHandler * game_ini_handler = NULL;
    IniHandler * static_ini_handler = NULL;
    int laps = 1;
    UiText * text_laps = NULL;

    void update_laps(int change);
};

#endif // MENU_SUNDAY_CUP_HPP