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

#include "screens/screens.h"

void setupButtons() {
  backButton.setup(2);
  forwardButton.setup(3);
  upButton.setup(4);
  downButton.setup(5);
  menuButton.setup(6, false);
  displayButton.setup(7, false);

  menuButton.onClick([](int pin, void* params) {
    IScreen* currentScreen = getCurrentScreen();
    if(currentScreen && !currentScreen->getName().compareTo("MenuScreen")) {
      setCurrentScreen(new DisplayTimeScreen(currentScreen->getDisplay()));
    } else {
      setCurrentScreen(new MenuScreen(currentScreen->getDisplay()));
    }
  });

  displayButton.onClick([](int pin, void* params) {
    static bool displayOn = true;
    IScreen* currentScreen = getCurrentScreen();
    if(!currentScreen) return;
    DisplayControl* display = currentScreen->getDisplay();
    
    displayOn = !displayOn;
    if(displayOn) currentScreen->begin(display);
    display->shouldDisplay(displayOn);
  });
}

#endif