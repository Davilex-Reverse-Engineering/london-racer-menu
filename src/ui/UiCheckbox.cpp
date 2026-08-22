#include "UiCheckbox.hpp"

UiCheckbox::UiCheckbox(float x, float y, bool enabled, Action action)
{
  this->rect.x = x;
  this->rect.y = y;
  this->rect.w = 24;
  this->rect.h = 24;
  this->enabled = enabled;
  this->action = action;
  this->selectable = true;

  // Set verts for cross which is shown when marked
  SDL_FColor cross_color = {1.0f, 1.0f, 0.0f, 1.0f};

  // Top left
  verts[0].position.x = this->rect.x;
  verts[0].position.y = this->rect.y + 4;
  verts[0].color = cross_color;

  verts[1].position.x = this->rect.x;
  verts[1].position.y = this->rect.y;
  verts[1].color = cross_color;

  verts[2].position.x = this->rect.x + 4;
  verts[2].position.y = this->rect.y;
  verts[2].color = cross_color;

  // To the middle from top left
  verts[3].position.x = verts[0].position.x;
  verts[3].position.y = verts[0].position.y;
  verts[3].color = cross_color;

  verts[4].position.x = verts[2].position.x;
  verts[4].position.y = verts[2].position.y;
  verts[4].color = cross_color;

  verts[5].position.x = this->rect.x + (this->rect.w / 2);
  verts[5].position.y = this->rect.y + (this->rect.h / 2);
  verts[5].color = cross_color;

  // Top right
  verts[6].position.x = this->rect.x + this->rect.w;
  verts[6].position.y = this->rect.y + 4;
  verts[6].color = cross_color;

  verts[7].position.x = this->rect.x + this->rect.w;
  verts[7].position.y = this->rect.y;
  verts[7].color = cross_color;

  verts[8].position.x = this->rect.x + this->rect.w - 4;
  verts[8].position.y = this->rect.y;
  verts[8].color = cross_color;

  // To the middle from top right
  verts[9].position.x = verts[6].position.x;
  verts[9].position.y = verts[6].position.y;
  verts[9].color = cross_color;

  verts[10].position.x = verts[8].position.x;
  verts[10].position.y = verts[8].position.y;
  verts[10].color = cross_color;

  verts[11].position.x = this->rect.x + (this->rect.w / 2);
  verts[11].position.y = this->rect.y + (this->rect.h / 2);
  verts[11].color = cross_color;

  // Bottom right
  verts[12].position.x = this->rect.x + this->rect.w;
  verts[12].position.y = this->rect.y + this->rect.h - 4;
  verts[12].color = cross_color;

  verts[13].position.x = this->rect.x + this->rect.w;
  verts[13].position.y = this->rect.y + this->rect.h;
  verts[13].color = cross_color;

  verts[14].position.x = this->rect.x + this->rect.w - 4;
  verts[14].position.y = this->rect.y + this->rect.h;
  verts[14].color = cross_color;

  // To the middle from bottom right
  verts[15].position.x = verts[12].position.x;
  verts[15].position.y = verts[12].position.y;
  verts[15].color = cross_color;

  verts[16].position.x = verts[14].position.x;
  verts[16].position.y = verts[14].position.y;
  verts[16].color = cross_color;

  verts[17].position.x = this->rect.x + (this->rect.w / 2);
  verts[17].position.y = this->rect.y + (this->rect.h / 2);
  verts[17].color = cross_color;

  // Bottom left
  verts[18].position.x = this->rect.x + 4;
  verts[18].position.y = this->rect.y + this->rect.h;
  verts[18].color = cross_color;

  verts[19].position.x = this->rect.x;
  verts[19].position.y = this->rect.y + this->rect.h;
  verts[19].color = cross_color;

  verts[20].position.x = this->rect.x;
  verts[20].position.y = this->rect.y + this->rect.h - 4;
  verts[20].color = cross_color;

  // To the middle from bottom right
  verts[21].position.x = verts[18].position.x;
  verts[21].position.y = verts[18].position.y;
  verts[21].color = cross_color;

  verts[22].position.x = verts[20].position.x;
  verts[22].position.y = verts[20].position.y;
  verts[22].color = cross_color;

  verts[23].position.x = this->rect.x + (this->rect.w / 2);
  verts[23].position.y = this->rect.y + (this->rect.h / 2);
  verts[23].color = cross_color;
}

UiCheckbox::~UiCheckbox()
{
}

void UiCheckbox::draw(SDL_Renderer *renderer, bool selected)
{
  if (selected) {
    SDL_SetRenderDrawColor(renderer, 115, 182, 32, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 29, 51, 9, 255);
  }
  SDL_RenderRect(renderer, &this->rect);
  if (this->enabled) {
    SDL_RenderGeometry(renderer, NULL, verts, 24, NULL, 0);
  }
}
