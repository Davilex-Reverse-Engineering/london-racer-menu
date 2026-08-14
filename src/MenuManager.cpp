#include "MenuManager.hpp"

#include "menu/RaceMenu.hpp"

MenuManager::MenuManager()
{
  this->scene = new RaceMenu();
}

MenuManager::~MenuManager()
{
  if (this->background) {
    SDL_DestroyTexture(this->background);
  }
  if (this->scene) {
    delete this->scene;
  }
}

Action MenuManager::update(std::vector<Input> inputs)
{
  Action action = scene->update(inputs);
  if (action == Action::QUIT) {
    return action;
  }
  return Action::NONE;
}

void MenuManager::draw(SDL_Renderer *renderer)
{
  if (!this->background && !background_loaded) {
    SDL_Surface * background_surface = SDL_LoadBMP("BKGR.bmp");
    if (background_surface) {
      this->background = SDL_CreateTextureFromSurface(renderer, background_surface);
      SDL_DestroySurface(background_surface);
      if (!this->background) {
        SDL_Log("Could not load image BKGR.bmp as texture");
      }
    } else {
      SDL_Log("Could not load image BKGR.bmp");
    }
    background_loaded = true;
  }
  if (this->background) {
    SDL_RenderTexture(renderer, this->background, NULL, NULL);
  }
  scene->draw(renderer);
}
