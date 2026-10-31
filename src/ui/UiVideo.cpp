#include "UiVideo.hpp"


UiVideo::UiVideo(float x, float y, float width, float height, const std::string &file_name)
{
  this->rect.x = x;
  this->rect.y = y;
  this->rect.w = width;
  this->rect.h = height;
  this->action = Action::NONE;
  this->selectable = false;

  this->video_handler.load(file_name);
}

UiVideo::~UiVideo()
{
}

void UiVideo::draw(SDL_Renderer *renderer, bool selected)
{
  SDL_Texture * texture = this->video_handler.getTexture(renderer);
  if (!texture) {
    return;
  }
  SDL_FRect srect = {0.0f, 0.0f, this->rect.w, this->rect.h};
  float texture_w = 0;
  float texture_h = 0;
  SDL_GetTextureSize(texture, &texture_w, &texture_h);
  srect.x = (texture_w - this->rect.w) / 2;
  srect.y = (texture_h - this->rect.h) / 2;
  SDL_RenderTexture(renderer, texture, &srect, &this->rect);
}

void UiVideo::setVideo(const std::string &file_name)
{
  this->video_handler.load(file_name);
}
