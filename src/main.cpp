#include <iostream>

#include "../vendor/SDL/include/SDL3/SDL.h"
#include "../vendor/SDL/include/SDL3/SDL_main.h"

#include "InputManager.hpp"
#include "MenuManager.hpp"
#include "IniHandler.hpp"

#define EXIT_CODE_QUIT 255
#define EXIT_CODE_START 40

void update_menu_ini(IniHandler * menu_ini_handler, int exit_code) {
  int music_count = menu_ini_handler->getInt("music", "count");
  int last_music =  menu_ini_handler->getInt("music", "last");

  last_music++;
  if (last_music >= music_count) {
    last_music = 0;
  }

  menu_ini_handler->setValue("music", "last", last_music);
  menu_ini_handler->setValue("general", "exitcode", exit_code);
}

int main(int argc, char** argv) {
  IniHandler menu_ini_handler = IniHandler();
  if (!menu_ini_handler.load("menu.ini")) {
    SDL_Log("Could not load menu.ini");
    return 1;
  }

  std::string argument = "";
  if (argc > 1) {
    argument = std::string(argv[1]);
  }
  if (argument == "/Default") {
    menu_ini_handler.setValue("general", "firsttime", "0");
  } else if (argument == "/FirstTime") {
    menu_ini_handler.setValue("general", "firsttime", "1");
  } else {
    SDL_Log("Please provide either /FirstTime or /Default as argument");
    return 2;
  }

  IniHandler game_ini_handler = IniHandler();
  if (!game_ini_handler.load("game.ini")) {
    SDL_Log("Could not load game.ini");
    return 3;
  }

  IniHandler static_ini_handler = IniHandler();
  if (!static_ini_handler.load("static.ini")) {
    SDL_Log("Could not load static.ini");
    return 4;
  }

  if(!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return 5;
  }

  SDL_Window * window = NULL;
  SDL_Renderer * renderer = NULL;
  if (!SDL_CreateWindowAndRenderer("London Racer", 640, 480, 0, &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    SDL_Quit();
    return 6;
  }
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  InputManager input_manager = InputManager();
  MenuManager menu_manager = MenuManager(&menu_ini_handler, &game_ini_handler, &static_ini_handler);

  bool running = true;
  Action action;
  while (running) { 
    std::vector<Input> inputs = input_manager.getInputs();
    for(Input input : inputs) {
      if (input.event == InputEvent::QUIT) {
        update_menu_ini(&menu_ini_handler, EXIT_CODE_QUIT);
        running = false;
        break;
      }
    }
    action = menu_manager.update(inputs);
    if (action == Action::QUIT) {
      update_menu_ini(&menu_ini_handler, EXIT_CODE_QUIT);
      running = false;
      break;
    } else if (action == Action::START) {
      update_menu_ini(&menu_ini_handler, EXIT_CODE_START);
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

  // Save the 2 ini files which the menu edits
  menu_ini_handler.write();
  game_ini_handler.write();

  // Cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}