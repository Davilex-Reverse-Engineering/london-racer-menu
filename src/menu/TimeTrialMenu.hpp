#ifndef MENU_TIME_TRIAL_MENU_HPP
#define MENU_TIME_TRIAL_MENU_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"
#include "../ui/UiText.hpp"
#include "../ui/UiImage.hpp"
#include "../ui/UiCheckbox.hpp"
#include "../IniHandler.hpp"

class TimeTrialMenu : public BaseMenu {
public:
    TimeTrialMenu(IniHandler * game_ini_handler, IniHandler * static_ini_handler);
    ~TimeTrialMenu();

    Action update(std::vector<Input> inputs);
private:
    IniHandler * game_ini_handler = NULL;
    IniHandler * static_ini_handler = NULL;
    int track = 0;
    UiText * text_car = NULL;
    UiText * text_laps = NULL;
    UiImage * track_image = NULL;
    UiCheckbox * checkbox_damage = NULL;

    int getNrLaps();
    void changeCar(int change);
    void setCar(int car_id);
    void setTrack(int track);
    void changeTrack(int change);
};

#endif // MENU_TIME_TRIAL_MENU_HPP