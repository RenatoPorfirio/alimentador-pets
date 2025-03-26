#ifndef _SCREENS_H_
#define _SCREENS_H_

#include "../display-control.h"
#include "../LDE.h"

/**
  * Estrutura de contexto da tela
  * - name: nome da tela
  * - optionsQnt: quantidade de opções
  * - currentOption: opção atual
  * - shouldUpdateScreen: flag para atualização da tela
  * - shouldUpdateOption: flag para atualização da opção
  * - display: controlador do display
  */
typedef struct {
  String name;
  uint8_t optionsQnt;
  uint8_t currentOption;
  bool shouldUpdateScreen;
  bool shouldUpdateOption;
  DisplayControl* display;
} ScreenContext;

/**
  * Interface de tela
  * - begin: inicializa a tela
  * - render: renderiza a tela
  * - getName: retorna o nome da tela
  * - setup: configura a tela
  * - getOptions: retorna as opções da tela
  * - getOptionsQnt: retorna a quantidade de opções
  * - getDisplay: retorna o controlador do display
  * - screens: lista de telas
  * - currentScreen: tela atual
  * - currentScreenContext: contexto da tela atual
  */
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
  4,
  0,
  true,
  true,
  nullptr
};

/**
  * Define a tela atual
  */
void setCurrentScreen(IScreen* newScreen) {
  if(IScreen::currentScreen) delete IScreen::currentScreen;
  IScreen::currentScreen = newScreen;
}

/**
  * Retorna a tela atual
  */
IScreen* getCurrentScreen() {
  return IScreen::currentScreen;
}

/**
  * Retorna o contexto da tela atual
  */
ScreenContext* getContext() {
  return &IScreen::currentScreenContext;
}

/**
  * Tela de menu
  * - options: opções do menu
  * - optionsQnt: quantidade de opções
  * - display: controlador do display
  */
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

/**
  * Tela de exibição de horários
  * - optionsQnt: quantidade de opções
  * - display: controlador do display
  */
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

/**
  * Tela de exibição de horários agendados
  * - optionsQnt: quantidade de opções
  * - display: controlador do display
  */
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

/**
  * Tela de remoção de horário
  * - optionsQnt: quantidade de opções
  * - display: controlador do display
  */
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

/**
  * Tela de adição de horário
  * - options: opções de horário
  * - optionsQnt: quantidade de opções
  * - display: controlador do display
  */
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

/**
  * Tela de configuração de ação
  * - optionsQnt: quantidade de opções
  * - display: controlador do display
  */
class ActionSetupScreen : public IScreen {
  uint8_t optionsQnt;
  DisplayControl* display;
  
  public:
  ActionSetupScreen();
  ActionSetupScreen(DisplayControl* display);
  void begin(DisplayControl* display);
  String getName();
  String* getOptions();
  uint8_t getOptionsQnt();
  DisplayControl* getDisplay();
  void render();
  void setup();
};

#include "menu-screen.h" // Inclui a implementação tela de menu
#include "display-time-screen.h" // Inclui a implementação tela de exibição de horários
#include "display-schedules-screen.h" // Inclui a implementação tela de exibição de horários agendados
#include "remove-time-screen.h" // Inclui a implementação tela de remoção de horário
#include "add-time-screen.h" // Inclui a implementação tela de adição de horário
#include "action-setup-screen.h" // Inclui a implementação tela de configuração de ação

#endif