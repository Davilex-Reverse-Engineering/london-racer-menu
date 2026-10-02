#include "UiText.hpp"

UiText::UiText(float x, float y, std::string text) : UiText::UiText(x, y, text, {255, 255, 255, 255}, FontType::REGULAR)
{
}

UiText::UiText(float x, float y, std::string text, SDL_Color color) : UiText::UiText(x, y, text, color, FontType::REGULAR)
{
}

UiText::UiText(float x, float y, std::string text, SDL_Color color, FontType font_type)
{
  this->rect.x = x;
  this->rect.y = y;
  this->text_string = text;
  this->color = color;
  this->font_type = font_type;
  this->action = Action::NONE;
  this->selectable = false;
}

UiText::~UiText()
{
  if (this->texture) {
    SDL_DestroyTexture(this->texture);
  }
}

void UiText::draw(SDL_Renderer *renderer, bool selected)
{
  if (!this->texture) {
    this->texture = FontHandler::getInstance()->getTexture(renderer, this->text_string, this->font_type, this->color);
    SDL_GetTextureSize(this->texture, &this->rect.w, &this->rect.h);
  }
  SDL_RenderTexture(renderer, this->texture, NULL, &this->rect);
}

void UiText::setText(const std::string &text)
{
  this->text_string = text;
  SDL_DestroyTexture(this->texture);
  this->texture = NULL;
}
