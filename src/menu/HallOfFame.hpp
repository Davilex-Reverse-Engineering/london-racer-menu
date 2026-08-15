#ifndef MENU_HALL_OF_FAME_HPP
#define MENU_HALL_OF_FAME_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"

class HallOfFame : public BaseMenu {
public:
    HallOfFame(Menu last_menu);
    ~HallOfFame();

private:
    Action get_exit_action(Menu last_menu);
};

#endif // MENU_HALL_OF_FAME_HPP