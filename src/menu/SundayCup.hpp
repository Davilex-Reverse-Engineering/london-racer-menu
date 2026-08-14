#ifndef SUNDAY_CUP_HPP
#define SUNDAY_CUP_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"

class SundayCup : public BaseMenu {
public:
    SundayCup();
    ~SundayCup();

    Action update(std::vector<Input> inputs);
};

#endif // SUNDAY_CUP_HPP