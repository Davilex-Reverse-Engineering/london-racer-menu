#include "UiButton.hpp"

#include "../FontHandler.hpp"
#include "../ImageHandler.hpp"

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
  if (this->text_texture) {
    SDL_DestroyTexture(this->text_texture);
  }
}

void UiButton::draw(SDL_Renderer *renderer, bool selected)
{
  if (!this->button_texture) {
    this->button_texture = ImageHandler::getInstance()->getTexture(renderer, "resources/images/button.png");
  }
  if (selected) {
    SDL_SetRenderDrawColor(renderer, 71, 110, 23, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
  }
  SDL_RenderTexture(renderer, this->button_texture, NULL, &this->rect);
  if (!this->text_texture) {
    this->text_texture = FontHandler::getInstance()->getTexture(renderer, this->text_string, FontType::REGULAR);
    SDL_GetTextureSize(this->text_texture, &this->text_rect.w, &this->text_rect.h);
    this->text_rect.x = this->rect.x + (this->rect.w / 2) - (this->text_rect.w / 2);
    this->text_rect.y = this->rect.y + (this->rect.h / 2) - (this->text_rect.h / 2);
  }
  SDL_RenderTexture(renderer, this->text_texture, NULL, &this->text_rect);
}
