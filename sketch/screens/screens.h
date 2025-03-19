#ifndef _SCREENS_H_
#define _SCREENS_H_

#include "../display-control.h"
#include "../LDE.h"

typedef struct {
  String name;
  uint8_t optionsQnt;
  uint8_t currentOption;
  bool shouldUpdateScreen;
  bool shouldUpdateOption;
  DisplayControl* display;
} ScreenContext;

class IScreen {
  public:
  static LDE<IScreen*> screens;
  static IScreen* currentScreen;
  static ScreenContext currentScreenContext;
  virtual void begin(DisplayControl* display) = 0;
  virtual void render() = 0;
  virtual String getName() = 0;
  virtual void setup() = 0;
  virtual String* getOptions() = 0;
  virtual uint8_t getOptionsQnt() = 0;
  virtual DisplayControl* getDisplay() = 0;
};

LDE<IScreen*> IScreen::screens;
IScreen* IScreen::currentScreen = nullptr;

ScreenContext IScreen::currentScreenContext = {
  String("MenuScreen"),
  3,
  0,
  true,
  true,
  nullptr
};


void setCurrentScreen(IScreen* newScreen) {
  if(IScreen::currentScreen) delete IScreen::currentScreen;
  IScreen::currentScreen = newScreen;
}

IScreen* getCurrentScreen() {
  return IScreen::currentScreen;
}

ScreenContext* getContext() {
  return &IScreen::currentScreenContext;
}

class MenuScreen : public IScreen {
  String options[5];
  uint8_t optionsQnt;
  DisplayControl* display;
  
  public:
  MenuScreen();
  MenuScreen(DisplayControl* display);
  void begin(DisplayControl* display);
  String getName();
  String* getOptions();
  uint8_t getOptionsQnt();
  DisplayControl* getDisplay();
  void render();
  void setup();
};

class DisplayTimeScreen : public IScreen {
  uint8_t optionsQnt;
  DisplayControl* display;
  
  public:
  DisplayTimeScreen();
  DisplayTimeScreen(DisplayControl* display);
  void begin(DisplayControl* display);
  String getName();
  String* getOptions();
  uint8_t getOptionsQnt();
  DisplayControl* getDisplay();
  void render();
  void setup();
};

class DisplaySchedulesScreen : public IScreen {
  uint8_t optionsQnt;
  DisplayControl* display;
  
  public:
  DisplaySchedulesScreen();
  DisplaySchedulesScreen(DisplayControl* display);
  void begin(DisplayControl* display);
  String getName();
  String* getOptions();
  uint8_t getOptionsQnt();
  DisplayControl* getDisplay();
  void render();
  void setup();
};

class RemoveTimeScreen : public IScreen {
  uint8_t optionsQnt;
  DisplayControl* display;
  
  public:
  RemoveTimeScreen();
  RemoveTimeScreen(DisplayControl* display);
  void begin(DisplayControl* display);
  String getName();
  String* getOptions();
  uint8_t getOptionsQnt();
  DisplayControl* getDisplay();
  void render();
  void setup();
};

class AddTimeScreen : public IScreen {
  uint8_t options[2];
  uint8_t optionsQnt;
  DisplayControl* display;
  
  public:
  AddTimeScreen();
  AddTimeScreen(DisplayControl* display);
  void begin(DisplayControl* display);
  String getName();
  String* getOptions();
  uint8_t getOptionsQnt();
  DisplayControl* getDisplay();
  void render();
  void setup();
};

#include "menu-screen.h"
#include "display-time-screen.h"
#include "display-schedules-screen.h"
#include "remove-time-screen.h"
#include "add-time-screen.h"

#endif