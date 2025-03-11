#ifndef _BUTTON_CONFIG_H_
#define _BUTTON_CONFIG_H_

#include "click-button.h"
#include "LDE.h"

ClickButton backButton;
ClickButton forwardButton;
ClickButton upButton;
ClickButton downButton;
ClickButton menuButton;
ClickButton displayButton;

LDE<ClickButton*> buttons;

void setupButtons() {
  backButton.setup(2);
  forwardButton.setup(3);
  upButton.setup(4);
  downButton.setup(5);
  menuButton.setup(6);
  displayButton.setup(7);

  buttons
    .pushBack(&backButton)
    .pushBack(&forwardButton)
    .pushBack(&upButton)
    .pushBack(&downButton)
    .pushBack(&menuButton)
    .pushBack(&displayButton);
}

#endif