#ifndef MENU_SUNDAY_CUP_HPP
#define MENU_SUNDAY_CUP_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"
#include "../ui/UiText.hpp"
#include "../ui/UiImage.hpp"
#include "../IniHandler.hpp"

class SundayCup : public BaseMenu {
public:
    SundayCup(IniHandler * game_ini_handler, IniHandler * static_ini_handler);
    ~SundayCup();

    Action update(std::vector<Input> inputs);

private:
    IniHandler * game_ini_handler = NULL;
    IniHandler * static_ini_handler = NULL;
    int track = 0;
    bool has_laps = false;
    int laps = 2;
    UiText * text_car = NULL;
    UiText * text_laps = NULL;
    UiImage * track_image = NULL;

    void changeCar(int change);
    void setTrack(int track);
    void changeTrack(int change);
    void changeLaps(int change);
};

#endif // MENU_SUNDAY_CUP_HPP