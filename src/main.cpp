#include <iostream>

#include "../vendor/SDL/include/SDL3/SDL.h"
#include "../vendor/SDL/include/SDL3/SDL_main.h"

#include "scene/MainScene.hpp"

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

  Scene * scene = new MainScene();

  SDL_Surface * background_surface = SDL_LoadBMP("BKGR.bmp");
  if (!background_surface) {
    SDL_Log("Could not load image BKGR.bmp");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

  SDL_Texture * background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);
  SDL_DestroySurface(background_surface);
  if (!background_texture) {
    SDL_Log("Could not load image BKGR.bmp as texture");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

  bool running = true;
  SDL_Event event;
  while (running) { 
    // Process input
    if (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_EVENT_QUIT:
          // End the loop if the programs is being closed
          running = false;
          break;
      }
    }

    // Clear the screen
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, background_texture, NULL, NULL);
    scene->draw(renderer);

    // Draw everything on a white background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
  }
  SDL_DestroyTexture(background_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  delete scene;

  return 0;
}