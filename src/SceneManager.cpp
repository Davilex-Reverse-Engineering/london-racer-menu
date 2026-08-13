#include "SceneManager.hpp"

#include "scene/MainScene.hpp"

SceneManager::SceneManager()
{
  this->scene = new MainScene();
}

SceneManager::~SceneManager()
{
  if (this->background) {
    SDL_DestroyTexture(this->background);
  }
  if (this->scene) {
    delete this->scene;
  }
}

Action SceneManager::update(std::vector<Input> inputs)
{
  Action action = scene->update(inputs);
  if (action == Action::QUIT) {
    return action;
  }
  return Action::NONE;
}

void SceneManager::draw(SDL_Renderer *renderer)
{
  if (!this->background) {
    SDL_Surface * background_surface = SDL_LoadBMP("BKGR.bmp");
    if (!background_surface) {
      SDL_Log("Could not load image BKGR.bmp");
    }

    this->background = SDL_CreateTextureFromSurface(renderer, background_surface);
    SDL_DestroySurface(background_surface);
    if (!this->background) {
      SDL_Log("Could not load image BKGR.bmp as texture");
    }
  }
  if (this->background) {
    SDL_RenderTexture(renderer, this->background, NULL, NULL);
  }
  scene->draw(renderer);
}
