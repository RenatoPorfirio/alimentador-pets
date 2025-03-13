#ifndef _SCREENS_H_
#define _SCREENS_H_

#include "display-control.h"
#include "click-button.h"
#include "LDE.h"

typedef struct {
  String name;
  uint8_t qntOptions;
  uint8_t currentOption;
  bool shouldUpdateScreen;
  bool shouldUpdateOption;
} ScreenContext;

class IScreen {
  public:
  static LDE<IScreen*> screens;
  static ScreenContext currentScreenContext;
  virtual void begin(DisplayControl* display) = 0;
  virtual void render() = 0;
  virtual String getName() = 0;
  virtual void setupControls() = 0;
  virtual String* getOptions() = 0;
  virtual uint8_t getOptionsQnt() = 0;
};

LDE<IScreen*> IScreen::screens;
ScreenContext IScreen::currentScreenContext = {
  String("MenuScreen"),
  3,
  0,
  true,
  true
};

class MenuScreen : public IScreen {
  String options[3];
  uint8_t qntOptions;
  DisplayControl* display;
  
  public:
  MenuScreen() {
    IScreen::screens.pushBack((IScreen*)this);
  }

  void begin(DisplayControl* display) {
    qntOptions = 3;
    options[0] = String("Agendar horario");
    options[1] = String("Remover horario");
    options[2] = String("Config. relogio");
    this->display = display;
    setupControls();
  }

  void render() {
    if(IScreen::currentScreenContext.name.compareTo(getName())) {
      IScreen::currentScreenContext.name = String(getName());
      IScreen::currentScreenContext.qntOptions = qntOptions;
      IScreen::currentScreenContext.currentOption = 0;
      IScreen::currentScreenContext.shouldUpdateScreen = true;
      IScreen::currentScreenContext.shouldUpdateOption = true;
    }
    if(IScreen::currentScreenContext.shouldUpdateScreen) {
      Serial.println("rendering menu screen...");
      display->clear();
      display->printAt("Menu", 1, 1);
      for (uint8_t i = 0; i < qntOptions; i++) {
        display->setCursor(1, (i + 2) * 8 + 2);
        display->print(String(i + 1, 10) + ". ");
        display->print(options[i]);  
      }
      IScreen::currentScreenContext.shouldUpdateScreen = false;
    }
    if (IScreen::currentScreenContext.shouldUpdateOption) {
      Serial.println("updating menu screen option...");
      for(uint8_t i = 0; i < qntOptions; i++) {
        display->eraseFrame(118, (i + 2) * 8 + 2, 5, 8);
      }
      display->printAt("<", 118, (IScreen::currentScreenContext.currentOption + 2) * 8 + 2);
      IScreen::currentScreenContext.shouldUpdateOption = false;
    }
  }

  String getName() {
    return "MenuScreen";
  }

  void setupControls() {
    ClickButton::resetDynamicInstances();

    downButton.onClick([](int pin, void* params) {
      uint8_t qntOptions = IScreen::currentScreenContext.qntOptions;
      uint8_t currentOption = IScreen::currentScreenContext.currentOption;

      uint8_t nextOption = (currentOption + 1) % qntOptions;
      IScreen::currentScreenContext.currentOption = nextOption;
      IScreen::currentScreenContext.shouldUpdateOption = true;
    });

    upButton.onClick([](int pin, void* params) {
      uint8_t qntOptions = IScreen::currentScreenContext.qntOptions;
      uint8_t currentOption = IScreen::currentScreenContext.currentOption;
      uint8_t nextOption = currentOption == 0 ? qntOptions - 1 : --currentOption;
      IScreen::currentScreenContext.currentOption = nextOption;
      IScreen::currentScreenContext.shouldUpdateOption = true;
    });

  }

  String* getOptions() {
    return options;
  }

  uint8_t getOptionsQnt() {
    return qntOptions;
  }
};

void setupScreens(DisplayControl* display) {
  IScreen::screens.forEach([](IScreen* screen, void* params) {
    DisplayControl* display = (DisplayControl*)params;
    screen->begin(display);
  }, (void*)display);
}

#endif