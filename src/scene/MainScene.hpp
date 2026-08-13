#ifndef MAIN_SCENE_HPP
#define MAIN_SCENE_HPP

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "Scene.hpp"

class MainScene : public Scene {
public:
    MainScene();
    ~MainScene();

    void draw(SDL_Renderer * renderer);
};

#endif // MAIN_SCENE_HPP