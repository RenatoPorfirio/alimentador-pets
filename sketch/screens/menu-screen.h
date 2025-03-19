MenuScreen::MenuScreen() {
  IScreen::screens.pushBack((IScreen*)this);
}
  
MenuScreen::MenuScreen(DisplayControl* display) {
  MenuScreen();
  begin(display);
}
 
void MenuScreen::begin(DisplayControl* display) {
  optionsQnt = 5;
  options[0] = String("Listar horarios");
  options[1] = String("Agendar horario");
  options[2] = String("Remover horario");
  options[3] = String("Config. acao");
  options[4] = String("Config. relogio");
  this->display = display;
  setup();
}
  
String MenuScreen::getName() { return String("MenuScreen"); }
String* MenuScreen::getOptions() { return options; }
uint8_t MenuScreen::getOptionsQnt() { return optionsQnt; }
DisplayControl* MenuScreen::getDisplay() { return display; }
  
void MenuScreen::render() {
ScreenContext* context = getContext();
  if(context->shouldUpdateScreen) {
    Serial.println("rendering menu screen...");
    display->clear();
    display->printAt("Menu", 1, 1);
    for (uint8_t i = 0; i < optionsQnt; i++) {
      display->setCursor(1, (i + 2) * 8 + 2);
      display->print(String(i + 1, 10) + ". ");
      display->print(options[i]);  
    }
    context->shouldUpdateScreen = false;
  }
  if (context->shouldUpdateOption) {
    for(uint8_t i = 0; i < optionsQnt; i++) {
      display->eraseFrame(121, (i + 2) * 8 + 2, 5, 8);
    }
    display->printAt("<", 121, (context->currentOption + 2) * 8 + 2);
    context->shouldUpdateOption = false;
  }
}
  
void MenuScreen::setup() {
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
    setCurrentScreen(new DisplayTimeScreen((DisplayControl*)params));
  }, display);

  forwardButton.onClick([](int pin, void* params) {
    ScreenContext* context = getContext();
    switch(context->currentOption) {
      case 0:
        setCurrentScreen(new DisplaySchedulesScreen((DisplayControl*)params));
        break;
      case 1:
        setCurrentScreen(new AddTimeScreen((DisplayControl*)params));
        break;
      case 2:
        setCurrentScreen(new RemoveTimeScreen((DisplayControl*)params));
        break;
      default:
        break;
    }
  }, display);
}
  