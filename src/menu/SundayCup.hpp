#ifndef SUNDAY_CUP_HPP
#define SUNDAY_CUP_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "BaseMenu.hpp"
#include "../ui/UiText.hpp"

class SundayCup : public BaseMenu {
public:
    SundayCup();
    ~SundayCup();

    Action update(std::vector<Input> inputs);

private:
    int laps = 1;
    UiText * text_laps = NULL;
};

#endif // SUNDAY_CUP_HPP