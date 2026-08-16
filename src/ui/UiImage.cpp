#include "UiImage.hpp"

#include "../Utils.hpp"

UiImage::UiImage(float x, float y, float width, float height, const std::string &file_name)
{
  this->rect.x = x;
  this->rect.y = y;
  this->rect.w = width;
  this->rect.h = height;
  this->file_name = file_name;
  this->action = Action::NONE;
  this->selectable = false;
}

UiImage::~UiImage()
{
  if (image)
    SDL_DestroyTexture(image);
}

void UiImage::draw(SDL_Renderer *renderer, bool selected)
{
  if (!this->image && !image_loaded) {
    this->image = Utils::createTexture(renderer, this->file_name);
    this->image_loaded = true;
  }
  if (this->image) {
    SDL_RenderTexture(renderer, this->image, NULL, &this->rect);
  }
}

void UiImage::setImage(const std::string &file_name)
{
  this->file_name = file_name;
  SDL_DestroyTexture(this->image);
  this->image = NULL;
  this->image_loaded = false;
}
