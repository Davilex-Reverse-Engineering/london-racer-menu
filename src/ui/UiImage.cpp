#include "UiImage.hpp"

#include "../ImageHandler.hpp"

UiImage::UiImage(float x, float y, float width, float height, const std::string &file_name, bool use_transparency)
{
  this->rect.x = x;
  this->rect.y = y;
  this->rect.w = width;
  this->rect.h = height;
  this->file_name = file_name;
  this->use_transparency = use_transparency;
  this->action = Action::NONE;
  this->selectable = false;
}

UiImage::~UiImage()
{
}

void UiImage::draw(SDL_Renderer *renderer, bool selected)
{
  if (!this->image && !image_loaded) {
    this->image = ImageHandler::getInstance()->getTexture(renderer, this->file_name, this->use_transparency);
    this->image_loaded = true;
  }
  if (this->image) {
    SDL_RenderTexture(renderer, this->image, NULL, &this->rect);
  }
}

void UiImage::setImage(const std::string &file_name)
{
  this->file_name = file_name;
  this->image = NULL;
  this->image_loaded = false;
}
