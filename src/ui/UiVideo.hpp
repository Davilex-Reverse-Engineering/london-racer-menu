#ifndef UI_VIDEO_HPP
#define UI_VIDEO_HPP

#include <string>

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "UiElement.hpp"

class UiVideo : public UiElement {
public:
  UiVideo(float x, float y, float width, float height, const std::string &file_name = "");
  ~UiVideo();

  void draw(SDL_Renderer * renderer, bool selected = false);

  void setVideo(const std::string &file_name);

private:
  std::string file_name = "";
  SDL_Texture * image = NULL;
  bool image_loaded = false;
};

#endif // UI_VIDEO_HPP