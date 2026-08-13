#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include "../../vendor/SDL/include/SDL3/SDL.h"

#include "../ui/UiElement.hpp"
#include "../Action.hpp"
#include "../Input.hpp"

class Scene {
public:
    virtual ~Scene() {};

    virtual Action update(std::vector<Input> inputs) {
      return this->updateUiElements(inputs);
      // for (Input input : inputs) {
      //   if (input.type == InputType::BUTTON) {
      //     switch (input.event) {
      //     case InputEvent::BUTTON_LEFT:
      //       this->selected--;
      //       if (this->selected > this->ui_elements.size() - 1) {
      //         this->selected = this->ui_elements.size() - 1;
      //       }
      //       break;
      //     case InputEvent::BUTTON_RIGHT:
      //       /* code */
      //       break;
      //     default:
      //       break;
      //     }
      //   }

      // }
    };

    virtual void draw(SDL_Renderer *renderer) {
      for (size_t i = 0; i < ui_elements.size(); i++) {
        ui_elements[i]->draw(renderer, i == selected);
      }
    };

protected:
  std::vector<UiElement*> ui_elements;
  size_t selected = 0;

  Action updateUiElements(std::vector<Input> inputs) {
    for (Input input : inputs) {
      if (input.type == InputType::POSITIONED) {
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
        if (mouse_element && input.event == InputEvent::MOUSE_LEFT_RELEASED) {
          return mouse_element->getAction();
        }
      } else if (input.type == InputType::BUTTON) {
        if (input.event == InputEvent::BUTTON_CONFIRM) {
          return ui_elements[selected]->getAction();
        }
      }
    }
    return Action::NONE;
  };
};

#endif // SCENE_HPP