#include "../time-config.h"

DisplaySchedulesScreen::DisplaySchedulesScreen() {
  IScreen::screens.pushBack((IScreen*)this);
}
  
DisplaySchedulesScreen::DisplaySchedulesScreen(DisplayControl* display) {
  DisplaySchedulesScreen();
  begin(display);
}
  
void DisplaySchedulesScreen::begin(DisplayControl* display) {
  optionsQnt = 0;
  this->display = display;
  setup();
}

String DisplaySchedulesScreen::getName() { return String("DisplaySchedulesScreen"); }
String* DisplaySchedulesScreen::getOptions() { return nullptr; }
uint8_t DisplaySchedulesScreen::getOptionsQnt() { return optionsQnt; }
DisplayControl* DisplaySchedulesScreen::getDisplay() { return display; }
  
void DisplaySchedulesScreen::render() {
  static TimeData* timeReg = nullptr;
  static uint8_t timeRegQnt = 0;

  ScreenContext* context = getContext();
  if(context->shouldUpdateScreen) {
    context->shouldUpdateScreen = false;
    display->clear();
    timeReg = timeRegManager.getTimeReg();
    timeRegQnt = timeRegManager.getTimeRegQnt();
    if(timeRegQnt == 0) {
      display->printAt("Horario nao", 29, 17);
      display->printAt("configurado!", 28, 25);
      display->printAt("Va ao menu.", 30, 40);
    } else {
      display->printAt("Horarios agendados:", 1, 1);
      for(uint8_t i = 0; i < timeRegQnt; i++) {
        display->setCursor(1, (i + 2) * 8 + 2);
        display->print(String(i + 1, 10) + ". ");
        display->print(timeDataToString(timeReg[i]));
      }
    }
  }
}

void DisplaySchedulesScreen::setup() {
  ScreenContext* context = getContext();
  context->name = String(getName());
  context->optionsQnt = optionsQnt;
  context->currentOption = 0;
  context->shouldUpdateScreen = true;
  context->shouldUpdateOption = true;
  ClickButton::resetDynamicInstances();

  backButton.onClick([](int pin, void* params) {
    setCurrentScreen(new MenuScreen(getCurrentScreen()->getDisplay()));
  });

  forwardButton.onClick([](int pin, void* params) {
    setCurrentScreen(new MenuScreen(getCurrentScreen()->getDisplay()));
  });
}