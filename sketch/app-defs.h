#ifndef _APP_DEFS_H_
#define _APP_DEFS_H_

#include "display-control.h"
#include "enhanced-time-span.h"
#include "time-reg-manager.h"
#include "button-config.h"
#include "time-config.h"
#include "LDE.h"

DisplayControl display;

typedef struct {
  uint8_t index;
  String value;
} ScreenOption;

class IScreen {
  public:
  virtual void begin() = 0;
  virtual void render() = 0;
  virtual String getName() = 0;
  virtual void setupControls() = 0;
  virtual ScreenOption* getOptions() = 0;
  virtual uint8_t getOptionsQnt() = 0;
};

class Context {
  static IScreen* _currentScreen;
  public:
  static void setCurrentScreen(IScreen* screen);
  static IScreen* getCurrentScreen();
};


class MenuScreen : public IScreen {
  ScreenOption options[3];
  uint8_t qntOptions;
  bool shouldUpdate;

  public:
  void begin() override {
    shouldUpdate = true;
    qntOptions = 3;
    options[0] = {1, "Agendar horário"};
    options[1] = {2, "Remover horário"};
    options[2] = {3, "Configurar relógio"};
  }

  void render() override {
    if (!shouldUpdate) return;
    for (uint8_t i = 0; i < qntOptions; i++) {  
      Exemplo: Serial.println(options[i].value);
    }
    shouldUpdate = false;
  }

  String getName() override {
    return "MenuScreen";
  }

  void setupControls() override {
  }

  ScreenOption* getOptions() override {
    return options;
  }

  uint8_t getOptionsQnt() override {
    return qntOptions;
  }
};

IScreen* Context::_currentScreen = nullptr;

void Context::setCurrentScreen(IScreen* screen) {
  Context::_currentScreen = screen;
}

IScreen* Context::getCurrentScreen() {
  return Context::_currentScreen;
}

#endif