#include "UiCheckbox.hpp"

UiCheckbox::UiCheckbox(float x, float y, bool enabled, Action action)
{
  this->rect.x = x;
  this->rect.y = y;
  this->rect.w = 24;
  this->rect.h = 24;
  this->enabled = enabled;
  this->action = action;
  this->selectable = true;
}

UiCheckbox::~UiCheckbox()
{
}

void UiCheckbox::draw(SDL_Renderer *renderer, bool selected)
{
  if (selected) {
    SDL_SetRenderDrawColor(renderer, 115, 182, 32, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 29, 51, 9, 255);
  }
  SDL_RenderRect(renderer, &this->rect);
  if (this->enabled) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    // Draw the \ of the X 3 lines thick from left to the right
    SDL_RenderLine(renderer, this->rect.x + 1, this->rect.y + this->rect.h - 2, this->rect.x + this->rect.w - 2, this->rect.y + 1);
    SDL_RenderLine(renderer, this->rect.x + 1, this->rect.y + this->rect.h - 3, this->rect.x + this->rect.w - 3, this->rect.y + 1);
    SDL_RenderLine(renderer, this->rect.x + 2, this->rect.y + this->rect.h - 2, this->rect.x + this->rect.w - 2, this->rect.y + 2);

    // Draw the / of the X 3 lines thick from left to the right
    SDL_RenderLine(renderer, this->rect.x + 1, this->rect.y + 1, this->rect.x + this->rect.w - 2, this->rect.y + this->rect.h - 2);
    SDL_RenderLine(renderer, this->rect.x + 2, this->rect.y + 1, this->rect.x + this->rect.w - 2, this->rect.y + this->rect.h - 3);
    SDL_RenderLine(renderer, this->rect.x + 1, this->rect.y + 2, this->rect.x + this->rect.w - 3, this->rect.y + this->rect.h - 2);
  }
}
