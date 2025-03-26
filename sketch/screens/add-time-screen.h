#include "../time-config.h"

AddTimeScreen::AddTimeScreen() {
  IScreen::screens.pushBack((IScreen*)this);
}
  
AddTimeScreen::AddTimeScreen(DisplayControl* display) {
  AddTimeScreen();
  begin(display);
}
 
void AddTimeScreen::begin(DisplayControl* display) {
  optionsQnt = 2;
  options[0] = 0;
  options[1] = 0;
  this->display = display;
  setup();
}
  
String AddTimeScreen::getName() { return String("AddTimeScreen"); }
String* AddTimeScreen::getOptions() { return nullptr; }
uint8_t AddTimeScreen::getOptionsQnt() { return optionsQnt; }
DisplayControl* AddTimeScreen::getDisplay() { return display; }

void AddTimeScreen::render() {
    ScreenContext* context = getContext();
    if(context->shouldUpdateScreen) {
      context->shouldUpdateScreen = false;
      display->clear();

      if(timeRegManager.isFull()) {
        display->printAt("Nao e possivel", 21, 12);
      display->printAt("agendar mais", 25, 21);
      display->printAt("horarios", 36, 31);
        display->printAt("Limite: ", 33, 44);
        display->print(String(TIME_REG_LIMIT));
        context->shouldUpdateOption = false;
        return;
      }
      
      display->printAt("Agendando um horario:", 1, 1);
      display->printBiggerAt(":00", 57, 27);
      display->printAt("hh    mm", 39, 43);
    }
    
    if(context->shouldUpdateOption) {
      context->shouldUpdateOption = false;
        display->eraseFrame(37, 17, 60, 8);
      if(context->currentOption < 100) {
        display->eraseFrame(34, 27, 24, 16);
        display->printBiggerAt(String(context->currentOption < 10 ? "0" : "") + String(context->currentOption, 10), 34, 27);
        display->printAt("v v", 37, 17);
      } else {
        display->eraseFrame(69, 17, 24, 26);
        display->printBiggerAt(String(context->currentOption < 110 ? "0" : "") + String(context->currentOption - 100, 10), 69, 27);
        display->printAt("v v", 71, 17);
      }
    }
}
  
void AddTimeScreen::setup() {
  ScreenContext* context = getContext();
  context->name = String(getName());
  context->optionsQnt = optionsQnt;
  context->currentOption = 0;
  context->shouldUpdateScreen = true;
  context->shouldUpdateOption = true;
  context->display = display;
  ClickButton::resetDynamicInstances();

    if(timeRegManager.isFull()) {
      backButton.onClick([](int pin, void* params) {
      setCurrentScreen(new MenuScreen(getCurrentScreen()->getDisplay()));
    });

    forwardButton.onClick([](int pin, void* params) {
      setCurrentScreen(new MenuScreen(getCurrentScreen()->getDisplay()));
    });
    return;
  }

  upButton.onClick([](int pin, void* params) {
    ScreenContext* context = getContext();
    uint8_t optionsQnt = context->optionsQnt;
    uint8_t currentOption = context->currentOption;
    if(currentOption < 100) {
      context->currentOption = (currentOption + 1) % 24;
    } else {
        context->currentOption = 100 + (currentOption - 99) % 60;
    }
    context->shouldUpdateOption = true;
  }, nullptr);

  downButton.onClick([](int pin, void* params) {
    ScreenContext* context = getContext();
    uint8_t optionsQnt = context->optionsQnt;
    uint8_t currentOption = context->currentOption;
    if(currentOption < 100) {
      context->currentOption = currentOption == 0 ? 23 : (currentOption - 1) % 24;
    } else {
        context->currentOption = currentOption == 100 ? 159 : 100 + (currentOption - 101) % 60;
    }
    context->shouldUpdateOption = true;
  }, nullptr);
  
  forwardButton.onClick([](int pin, void* params) {
    ScreenContext* context = getContext();
    uint8_t optionsQnt = context->optionsQnt;
    uint8_t currentOption = context->currentOption;
    uint8_t* options = (uint8_t*)params;
    if(currentOption < 100) {
      options[0] = currentOption;
      context->currentOption = options[1] + 100;
      context->shouldUpdateOption = true;
    } else {
      options[1] = currentOption - 100;
      TimeData timeData = {options[0], options[1], 0};
      addTime(timeData);
      setCurrentScreen(new DisplayTimeScreen(context->display));
    }
  }, (void*)options);

  backButton.onClick([](int pin, void* params) {
    ScreenContext* context = getContext();
    uint8_t optionsQnt = context->optionsQnt;
    uint8_t currentOption = context->currentOption;
    uint8_t* options = (uint8_t*)params;
    if(currentOption < 100) {
      setCurrentScreen(new MenuScreen(context->display));
    } else {
      options[1] = currentOption - 100;
      context->currentOption = options[0];
      context->shouldUpdateOption = true;
    }
  }, (void*)options);
}
