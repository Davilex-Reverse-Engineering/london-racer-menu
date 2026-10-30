#include "UiVideo.hpp"

#include "../VideoHandler.hpp"

UiVideo::UiVideo(float x, float y, float width, float height, const std::string &file_name)
{
  this->rect.x = x;
  this->rect.y = y;
  this->rect.w = width;
  this->rect.h = height;
  this->file_name = file_name;
  this->action = Action::NONE;
  this->selectable = false;
}

UiVideo::~UiVideo()
{
  SDL_DestroyTexture(image);
}

void UiVideo::draw(SDL_Renderer *renderer, bool selected)
{
  if (!this->image && !image_loaded) {
    this->image = VideoHandler::getInstance()->getTexture(renderer, file_name);
    this->image_loaded = true;
  }
  if (this->image) {
    SDL_FRect srect = {0.0f, 0.0f, this->rect.w, this->rect.h};
    float texture_w = 0;
    float texture_h = 0;
    SDL_GetTextureSize(this->image, &texture_w, &texture_h);
    srect.x = (texture_w - this->rect.w) / 2;
    srect.y = (texture_h - this->rect.h) / 2;
    SDL_RenderTexture(renderer, this->image, &srect, &this->rect);
  }
}

void UiVideo::setVideo(const std::string &file_name)
{
  this->file_name = file_name;
  SDL_DestroyTexture(this->image);
  this->image = NULL;
  this->image_loaded = false;
}
