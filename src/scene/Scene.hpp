#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "../ui/UiElement.hpp"

class Scene {
public:
    virtual ~Scene() {};

    virtual void draw(SDL_Renderer *renderer) = 0;
protected:
    std::vector<UiElement*> ui_elements;
};

#endif // SCENE_HPP