#include "UiRectangle.hpp"

UiRectangle::UiRectangle(float x, float y, float width, float height, SDL_Color color)
{
  this->rect.x = x;
  this->rect.y = y;
  this->rect.w = width;
  this->rect.h = height;
  this->color = color;
  this->action = Action::NONE;
  this->selectable = false;
}

UiRectangle::~UiRectangle()
{
}

void UiRectangle::draw(SDL_Renderer *renderer, bool selected)
{
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(renderer, &this->rect);
}
