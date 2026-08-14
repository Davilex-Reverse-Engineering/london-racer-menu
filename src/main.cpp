#include <iostream>

#include "../vendor/SDL/include/SDL3/SDL.h"
#include "../vendor/SDL/include/SDL3/SDL_main.h"

#include "InputManager.hpp"
#include "MenuManager.hpp"

int main(int argc, char** argv) {
  (void) argc;
  (void) argv;

  if(!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return 1;
  }

  SDL_Window * window = NULL;
  SDL_Renderer * renderer = NULL;
  if (!SDL_CreateWindowAndRenderer("London Racer", 640, 480, 0, &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    SDL_Quit();
    return 2;
  }

  InputManager input_manager = InputManager();
  MenuManager menu_manager = MenuManager();

  bool running = true;
  Action action;
  while (running) { 
    std::vector<Input> inputs = input_manager.getInputs();
    for(Input input : inputs) {
      if (input.event == InputEvent::QUIT) {
        running = false;
        break;
      }
    }
    action = menu_manager.update(inputs);
    if (action == Action::QUIT) {
      running = false;
      break;
    }

    // Clear the screen
    SDL_RenderClear(renderer);
    menu_manager.draw(renderer);

    // Draw everything on a white background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}