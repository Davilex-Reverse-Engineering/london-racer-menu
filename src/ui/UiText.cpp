#include "UiText.hpp"

UiText::UiText(float x, float y, std::string text, SDL_Color color)
{
  this->rect.x = x;
  this->rect.y = y;
  this->text_string = text;
  this->color = color;
  this->action = Action::NONE;
  this->selectable = false;
}

UiText::~UiText()
{
}

void UiText::draw(SDL_Renderer *renderer, bool selected)
{
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDebugText(renderer, this->rect.x, this->rect.y, text_string.c_str());
}
