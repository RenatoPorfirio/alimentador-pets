#ifndef _SCREENS_H_
#define _SCREENS_H_

#include "display-control.h"
#include "time-config.h"
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
  true
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
  String options[3];
  uint8_t qntOptions;
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
  uint8_t qntOptions;
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

// MENU ------------------------------------------------------------------------------------------------------------------------------------------
MenuScreen::MenuScreen() {
  IScreen::screens.pushBack((IScreen*)this);
}

MenuScreen::MenuScreen(DisplayControl* display) {
  MenuScreen();
  begin(display);
}

void MenuScreen::begin(DisplayControl* display) {
  qntOptions = 3;
  options[0] = String("Agendar horario");
  options[1] = String("Remover horario");
  options[2] = String("Config. relogio");
  this->display = display;
  setup();
}

String MenuScreen::getName() { return String("MenuScreen"); }
String* MenuScreen::getOptions() { return options; }
uint8_t MenuScreen::getOptionsQnt() { return qntOptions; }
DisplayControl* MenuScreen::getDisplay() { return display; }

void MenuScreen::render() {
  ScreenContext* context = getContext();
  if(context->shouldUpdateScreen) {
    Serial.println("rendering menu screen...");
    display->clear();
    display->printAt("Menu", 1, 1);
    for (uint8_t i = 0; i < qntOptions; i++) {
      display->setCursor(1, (i + 2) * 8 + 2);
      display->print(String(i + 1, 10) + ". ");
      display->print(options[i]);  
    }
    context->shouldUpdateScreen = false;
  }
  if (context->shouldUpdateOption) {
    Serial.println("updating menu screen option...");
    for(uint8_t i = 0; i < qntOptions; i++) {
      display->eraseFrame(118, (i + 2) * 8 + 2, 5, 8);
    }
    display->printAt("<", 118, (context->currentOption + 2) * 8 + 2);
    context->shouldUpdateOption = false;
  }
}

void MenuScreen::setup() {
  ScreenContext* context = getContext();
  context->name = String(getName());
  context->qntOptions = qntOptions;
  context->currentOption = 0;
  context->shouldUpdateScreen = true;
  context->shouldUpdateOption = true;
  ClickButton::resetDynamicInstances();

  downButton.onClick([](int pin, void* params) {
    ScreenContext* context = getContext();
    uint8_t qntOptions = context->qntOptions;
    uint8_t currentOption = context->currentOption;

    uint8_t nextOption = (currentOption + 1) % qntOptions;
    context->currentOption = nextOption;
    context->shouldUpdateOption = true;
  });

  upButton.onClick([](int pin, void* params) {
    ScreenContext* context = getContext();
    uint8_t qntOptions = context->qntOptions;
    uint8_t currentOption = context->currentOption;
    uint8_t nextOption = currentOption == 0 ? qntOptions - 1 : --currentOption;
    context->currentOption = nextOption;
    context->shouldUpdateOption = true;
  });

  backButton.onClick([](int pin, void* params) {
    setCurrentScreen(new DisplayTimeScreen((DisplayControl*)params));
  }, display);
}



//DisplayTime ------------------------------------------------------------------------------------------------------------------
DisplayTimeScreen::DisplayTimeScreen() {
  IScreen::screens.pushBack((IScreen*)this);
}

DisplayTimeScreen::DisplayTimeScreen(DisplayControl* display) {
  DisplayTimeScreen();
  begin(display);
}

void DisplayTimeScreen::begin(DisplayControl* display) {
  qntOptions = 0;
  this->display = display;
  setup();
}

String DisplayTimeScreen::getName() { return String("DisplayTimeScreen"); }
String* DisplayTimeScreen::getOptions() { return nullptr; }
uint8_t DisplayTimeScreen::getOptionsQnt() { return qntOptions; }
DisplayControl* DisplayTimeScreen::getDisplay() { return display; }

void DisplayTimeScreen::render() {
  static EnhancedTimeSpan timeSpan;
  static double lastTime = 0;
  double currentTime = millis();

  ScreenContext* context = getContext();
  if(context->shouldUpdateScreen) {
    display->clear();
    if(timeQueue.size() == 0) {
      display->printAt("Horario nao", 29, 17);
      display->printAt("configurado!", 28, 25);
      display->printAt("Va ao menu.", 30, 40);
    } else {
      DateTime* nextDateTime = timeQueue.first();
      DateTime now = rtc.now();

      timeSpan = EnhancedTimeSpan(*nextDateTime - now);
      display->printAt("Tempo restante para oproximo acionamento:", 1, 1);
    }
    context->shouldUpdateScreen = false;
  }
  if(timeQueue.size() > 0 && currentTime - lastTime > 1000) {
    timeSpan.decrease(1);
    display->eraseFrame(1, 25, 48, 8);
    display->printAt(timeSpan.toString(), 1, 25);
    lastTime = currentTime;
  }
}

void DisplayTimeScreen::setup() {
  ScreenContext* context = getContext();
  context->name = String(getName());
  context->qntOptions = qntOptions;
  context->currentOption = 0;
  context->shouldUpdateScreen = true;
  context->shouldUpdateOption = true;
  ClickButton::resetDynamicInstances();
}

#endif