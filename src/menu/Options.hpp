#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"

class Options : public BaseMenu {
public:
    Options();
    ~Options();

    Action update(std::vector<Input> inputs);
};

#endif // OPTIONS_HPP