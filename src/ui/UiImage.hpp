#ifndef UI_IMAGE_HPP
#define UI_IMAGE_HPP

#include <string>

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "UiElement.hpp"

class UiImage : public UiElement {
public:
  UiImage(float x, float y, float width, float height, const std::string &file_name = "");
  ~UiImage();

  void draw(SDL_Renderer * renderer, bool selected = false);

  void setImage(const std::string &file_name);

private:
  std::string file_name = "";
  SDL_Texture * image = NULL;
  bool image_loaded = false;
};

#endif // UI_IMAGE_HPP