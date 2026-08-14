#ifndef LEAGUE_HPP
#define LEAGUE_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"

class League : public BaseMenu {
public:
    League(int number);
    ~League();

    Action update(std::vector<Input> inputs);

private:
    int number = 1;
};

#endif // LEAGUE_HPP