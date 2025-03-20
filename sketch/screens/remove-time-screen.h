RemoveTimeScreen::RemoveTimeScreen() {
  IScreen::screens.pushBack((IScreen*)this);
}
  
RemoveTimeScreen::RemoveTimeScreen(DisplayControl* display) {
  RemoveTimeScreen();
  begin(display);
}
 
void RemoveTimeScreen::begin(DisplayControl* display) { 
  optionsQnt = timeRegManager.getTimeRegQnt();
  this->display = display;
  setup();
}
  
String RemoveTimeScreen::getName() { return String("RemoveTimeScreen"); }
String* RemoveTimeScreen::getOptions() { return nullptr; }
uint8_t RemoveTimeScreen::getOptionsQnt() { return optionsQnt; }
DisplayControl* RemoveTimeScreen::getDisplay() { return display; }

void RemoveTimeScreen::render() {
  static TimeData* timeReg = nullptr;
  
  ScreenContext* context = getContext();
  if(context->shouldUpdateScreen) {
    context->shouldUpdateScreen = false;
    display->clear();
    timeReg = timeRegManager.getTimeReg();
    
    if(optionsQnt == 0) {
      display->printAt("Horario nao", 29, 17);
      display->printAt("configurado!", 28, 25);
      display->printAt("Va ao menu.", 30, 40);
    } else {
      display->printAt("Selecione um horario para remover:", 1, 1);
      for(uint8_t i = 0; i < optionsQnt; i++) {
        display->setCursor(1, (i + 2) * 8 + 6);
        display->print(String(i + 1, 10) + ". ");
        display->print(timeDataToString(timeReg[i]));
      }
    }
  }
  if (context->shouldUpdateOption) {
    for(uint8_t i = 0; i < optionsQnt; i++) {
      display->eraseFrame(121, (i + 2) * 8 + 6, 5, 8);
    }
    display->printAt("<", 121, (context->currentOption + 2) * 8 + 6);
    context->shouldUpdateOption = false;
  }
}
  
void RemoveTimeScreen::setup() {
  ScreenContext* context = getContext();
  context->name = String(getName());
  context->optionsQnt = optionsQnt;
  context->currentOption = 0;
  context->shouldUpdateScreen = true;
  context->shouldUpdateOption = true;
  context->display = display;
  ClickButton::resetDynamicInstances();

  downButton.onClick([](int pin, void* params) {
    ScreenContext* context = getContext();
    uint8_t optionsQnt = context->optionsQnt;
    uint8_t currentOption = context->currentOption;

    uint8_t nextOption = (currentOption + 1) % optionsQnt;
    context->currentOption = nextOption;
    context->shouldUpdateOption = true;
  });

  upButton.onClick([](int pin, void* params) {
    ScreenContext* context = getContext();
    uint8_t optionsQnt = context->optionsQnt;
    uint8_t currentOption = context->currentOption;
    uint8_t nextOption = currentOption == 0 ? optionsQnt - 1 : --currentOption;
    context->currentOption = nextOption;
    context->shouldUpdateOption = true;
  });

  backButton.onClick([](int pin, void* params) {
    setCurrentScreen(new MenuScreen((DisplayControl*)params));
  }, display);

  forwardButton.onClick([](int pin, void* params) {
    ScreenContext* context = getContext();
    TimeData* timeReg = timeRegManager.getTimeReg();

    uint8_t currentOption = context->currentOption;
    DateTime now = rtc.now();
    DateTime* dt = new DateTime(now.year(), now.month(), now.day(), timeReg[currentOption].hr, timeReg[currentOption].min, timeReg[currentOption].sec);
    EnhancedDateTime* result = timeQueue.remove(dt, [](DateTime* a, DateTime* b) {
      return a->hour() == b->hour() && a->minute() == b->minute() && a->second() == b->second();
    });
    delete result;
    timeRegManager.removeTimeData(currentOption);
    delete dt;
    timeRemainingUpdate();
    setCurrentScreen(new DisplaySchedulesScreen((DisplayControl*)params));
  }, display);
}
