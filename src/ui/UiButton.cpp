#include "UiButton.hpp"

UiButton::UiButton(float x, float y, float width, float height, std::string text, Action action)
{
  this->rect.x = x;
  this->rect.y = y;
  this->rect.w = width;
  this->rect.h = height;
  this->text_string = text;
  this->action = action;
  this->selectable = true;
}

UiButton::~UiButton()
{
}

void UiButton::draw(SDL_Renderer *renderer, bool selected)
{
  if (selected) {
    SDL_SetRenderDrawColor(renderer, 71, 110, 23, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
  }
  SDL_RenderFillRect(renderer, &this->rect);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDebugText(renderer, this->rect.x + (this->rect.w / 2) - (text_string.length() * 8 / 2), this->rect.y + (this->rect.h / 2 - 4), text_string.c_str());
}
