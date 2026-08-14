#ifndef BASE_MENU_HPP
#define BASE_MENU_HPP

#include <string>
#include <vector>

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "../ui/UiElement.hpp"
#include "../Action.hpp"
#include "../Input.hpp"

enum class Menu {
  RACE_MENU,
  TOURNAMENT_MENU,
  SUNDAY_CUP,
  TIME_TRIAL_MENU,
  SHOWROOM,
  GARAGE,
  HALL_OF_FAME,
  PLAYER_MENU,
  OPTIONS,
  SETTINGS,
  CONTROLS,
  TRACK_INFO,
  LEAGUE_ONE,
  LEAGUE_TWO,
  LEAGUE_THREE
};

class BaseMenu {
public:
    virtual ~BaseMenu() {
      for(UiElement * element : ui_elements) {
        if (element) {
          delete element;
        }
      }
    };

    virtual Action update(std::vector<Input> inputs) {
      return this->processInputs(inputs);
    };

    virtual void draw(SDL_Renderer *renderer) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderDebugText(renderer, 216.0f, 35.0f, title.c_str());
      for (size_t i = 0; i < ui_elements.size(); i++) {
        ui_elements[i]->draw(renderer, i == selected);
      }
    };

protected:
  std::vector<UiElement*> ui_elements;
  size_t selected = 0;
  std::string title = "";

  Action processButtonInput(Input input) {
    switch(input.event) {
      case InputEvent::BUTTON_CONFIRM:
        {
          Action action = ui_elements[selected]->getAction();
          if (action != Action::NONE) {
            return action;
          }
        }
        break;
      case InputEvent::BUTTON_LEFT:
        this->selected--;
        while (this->selected > this->ui_elements.size() || !this->ui_elements[selected]->isSelectable()) {
          if (this->selected > this->ui_elements.size()) {
            this->selected = this->ui_elements.size() - 1;
          } else {
            this->selected--;
          }
        }
        break;
      case InputEvent::BUTTON_RIGHT:
        this->selected++;
        while (this->selected >= this->ui_elements.size() || !this->ui_elements[selected]->isSelectable()) {
          if (this->selected >= this->ui_elements.size()) {
            this->selected = 0;
          } else {
            this->selected++;
          }
        }
        break;
      default:
        break;
    }
    return Action::NONE;
  };

  Action processPositionedInput(Input input) {
    UiElement * mouse_element = NULL;
    for(size_t i = 0; i < ui_elements.size(); i++) {
      if (ui_elements[i]->isPointOnElement(&input.position)) {
        mouse_element = ui_elements[i];
        if (mouse_element->isSelectable()) {
          selected = i;
        }
        break;
      }
    }
    Action action = Action::NONE;
    if (mouse_element && input.event == InputEvent::MOUSE_LEFT_RELEASED) {
      action = mouse_element->getAction();
      if(action != Action::NONE) {
        return action;
      }
    }
    return action;
  };

  Action processInputs(std::vector<Input> inputs) {
    Action action = Action::NONE;
    for (Input input : inputs) {
      switch(input.type) {
        case InputType::BUTTON:
          SDL_HideCursor();
          action = processButtonInput(input);
          if (action != Action::NONE) {
            return action;
          }
          break;
        case InputType::POSITIONED:
          SDL_ShowCursor();
          action = processPositionedInput(input);
          if (action != Action::NONE) {
            return action;
          }
          break;
        default:
          break;
      }
    }
    return action;
  };
};

#endif // BASE_MENU_HPP