#include "MainScene.hpp"

#include "../ui/UiButton.hpp"

MainScene::MainScene()
{
  this->ui_elements.push_back(new UiButton(329.0f, 103.0f, 300.0f, 104.0f, "Tournament", Action::NONE));
  this->ui_elements.push_back(new UiButton(215.0f, 102.0f, 100.0f, 50.0f, "Time Trial"));
  this->ui_elements.push_back(new UiButton(215.0f, 158.0f, 100.0f, 50.0f, "Sunday Cup"));
  this->ui_elements.push_back(new UiButton(10.0f, 360.0f, 100.0f, 50.0f, "Options"));
  this->ui_elements.push_back(new UiButton(10.0f, 415.0f, 100.0f, 50.0f, "Quit", Action::QUIT));
}

MainScene::~MainScene()
{
  for(UiElement * element : ui_elements) {
    delete element;
  }
}

Action MainScene::update(std::vector<Input> inputs)
{
  return this->updateUiElements(inputs);
}
