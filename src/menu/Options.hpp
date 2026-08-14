#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"

class Options : public BaseMenu {
public:
    Options(Menu last_menu);
    ~Options();

    Action update(std::vector<Input> inputs);

private:
    Action get_exit_action(Menu last_menu);
};

#endif // OPTIONS_HPP