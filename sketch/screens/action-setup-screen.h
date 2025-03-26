#include "../routine-config.h"

ActionSetupScreen::ActionSetupScreen() {
  IScreen::screens.pushBack((IScreen*)this);
}
  
ActionSetupScreen::ActionSetupScreen(DisplayControl* display) {
  ActionSetupScreen();
  begin(display);
}
 
void ActionSetupScreen::begin(DisplayControl* display) {
  optionsQnt = 0;
  this->display = display;
  setup();
}
  
String ActionSetupScreen::getName() { return String("ActionSetupScreen"); }
String* ActionSetupScreen::getOptions() { return nullptr; }
uint8_t ActionSetupScreen::getOptionsQnt() { return optionsQnt; }
DisplayControl* ActionSetupScreen::getDisplay() { return display; }

void ActionSetupScreen::render() {
    ScreenContext* context = getContext();
    if(context->shouldUpdateScreen) {
      context->shouldUpdateScreen = false;
      display->clear();

      display->printAt("Configure o tempo de acao:", 1, 1);
    }
    
    if(context->shouldUpdateOption) {
      uint8_t curr = context->currentOption;
      context->shouldUpdateOption = false;
      display->eraseFrame(40, 32, 57, 16);
      display->printBiggerAt(String(curr < 10 ? "00" : curr < 100 ? "0" : "") + String(curr) + "s", 40, 32);
    }
}
  
void ActionSetupScreen::setup() {
  ScreenContext* context = getContext();
  context->name = String(getName());
  context->optionsQnt = optionsQnt;
  context->currentOption = EEPROM.read(SECS_SETPOINT_ADDR);
  context->shouldUpdateScreen = true;
  context->shouldUpdateOption = true;
  context->display = display;
  ClickButton::resetDynamicInstances();

  downButton.onClick([](int pin, void* params) {
    ScreenContext* context = getContext();
    context->currentOption = context->currentOption == 1 ? 180 : (context->currentOption - 1);
    context->shouldUpdateOption = true;
  }, nullptr);

  upButton.onClick([](int pin, void* params) {
    ScreenContext* context = getContext();
    context->currentOption = context->currentOption == 181 ? 1 : (context->currentOption + 1);
    context->shouldUpdateOption = true;
  }, nullptr);
  
  forwardButton.onClick([](int pin, void* params) {
    ScreenContext* context = getContext();
    routineTimeConfig(context->currentOption);
    setCurrentScreen(new DisplayTimeScreen(context->display));
  }, nullptr);

  backButton.onClick([](int pin, void* params) {
    ScreenContext* context = getContext();
    setCurrentScreen(new MenuScreen(context->display));
  }, nullptr);
}
