#ifndef MENU_TOURNAMENT_MENU_HPP
#define MENU_TOURNAMENT_MENU_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"
#include "../IniHandler.hpp"

class TournamentMenu : public BaseMenu {
public:
    TournamentMenu(IniHandler * game_ini_handler);
    ~TournamentMenu();

    Action update(std::vector<Input> inputs);

private:
    IniHandler * game_ini_handler = NULL;
};

#endif // MENU_TOURNAMENT_MENU_HPP