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
  if (selected) {
    if (!this->button_selected_texture) {
      this->button_selected_texture = ImageHandler::getInstance()->getTexture(renderer, "resources/images/button_selected.png");
    }
    SDL_RenderTexture(renderer, this->button_selected_texture, NULL, &this->rect);
  } else {
    if (!this->button_texture) {
      this->button_texture = ImageHandler::getInstance()->getTexture(renderer, "resources/images/button.png");
    }
    SDL_RenderTexture(renderer, this->button_texture, NULL, &this->rect);
  }
  if (!this->text_texture) {
    this->text_texture = FontHandler::getInstance()->getTexture(renderer, this->text_string, FontType::REGULAR);
    SDL_GetTextureSize(this->text_texture, &this->text_rect.w, &this->text_rect.h);
    this->text_rect.x = this->rect.x + (this->rect.w / 2) - (this->text_rect.w / 2);
    this->text_rect.y = this->rect.y + (this->rect.h / 2) - (this->text_rect.h / 2);
  }
  SDL_RenderTexture(renderer, this->text_texture, NULL, &this->text_rect);
}
