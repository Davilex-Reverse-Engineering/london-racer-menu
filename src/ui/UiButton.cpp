#include "UiButton.hpp"

UiButton::UiButton(float x, float y, float width, float height, std::string text, bool selected)
{
  this->rect.x = x;
  this->rect.y = y;
  this->rect.w = width;
  this->rect.h = height;
  this->text_string = text;
  this->selected = selected;
}

UiButton::~UiButton()
{
}

void UiButton::draw(SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
  SDL_RenderFillRect(renderer, &this->rect);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDebugText(renderer, this->rect.x, this->rect.y, text_string.c_str());
}
