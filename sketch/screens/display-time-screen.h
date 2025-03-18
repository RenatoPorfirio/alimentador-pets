#include "../time-config.h"

DisplayTimeScreen::DisplayTimeScreen() {
  IScreen::screens.pushBack((IScreen*)this);
}
  
DisplayTimeScreen::DisplayTimeScreen(DisplayControl* display) {
  DisplayTimeScreen();
  begin(display);
}
  
void DisplayTimeScreen::begin(DisplayControl* display) {
  optionsQnt = 0;
  this->display = display;
  setup();
}
  
String DisplayTimeScreen::getName() { return String("DisplayTimeScreen"); }
String* DisplayTimeScreen::getOptions() { return nullptr; }
uint8_t DisplayTimeScreen::getOptionsQnt() { return optionsQnt; }
DisplayControl* DisplayTimeScreen::getDisplay() { return display; }
  
void DisplayTimeScreen::render() {
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
      display->printAt("Tempo restante para oproximo acionamento:", 1, 1);
    }
    context->shouldUpdateScreen = false;
  }
  if(timeQueue.size() > 0 && currentTime - lastTime > 1000) {
    display->eraseFrame(1, 25, 59, 8);
    display->printAt(timeRemaining.toString(false), 1, 25);
    lastTime = currentTime;
  }
}

void DisplayTimeScreen::setup() {
  ScreenContext* context = getContext();
  context->name = String(getName());
  context->optionsQnt = optionsQnt;
  context->currentOption = 0;
  context->shouldUpdateScreen = true;
  context->shouldUpdateOption = true;
  ClickButton::resetDynamicInstances();
}