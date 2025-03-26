#ifndef _CLICK_BUTTON_H_
#define _CLICK_BUTTON_H_

#include "LDE.h";

/**
 * Classe para controle de cliques de botões
 * - Permite configurar um callback para ser chamado quando o botão é pressionado
 * - Permite configurar um parâmetro para ser passado ao callback
 * - Permite configurar um botão como dinâmico, para ser resetado automaticamente
 * - Permite configurar um botão como estático, para ser resetado manualmente
 * Propriedades:
 * - pin: Pino do botão
 * - pressed: Estado do botão
 * - click: Callback para ser chamado quando o botão é pressionado
 * - clickParams: Parâmetro a ser passado ao callback
 * Métodos:
 * - setup: Configura o botão
 * - checkClick: Verifica se o botão foi pressionado
 * - onClick: Configura o callback a ser chamado quando o botão é pressionado
 * - getPin: Retorna o pino do botão
 * - checkAllClicks: Verifica se todos os botões foram pressionados
 * - resetDynamicInstances: Reseta todos os botões dinâmicos
 * - resetAllInstances: Reseta todos os botões estáticos
 * Propriedades estáticas:
 * - dynamicInstances: Lista de botões dinâmicos
 * - allInstances: Lista de todos os botões
 * Métodos estáticos:
 * - checkAllClicks: Verifica se todos os botões foram pressionados
 * - resetDynamicInstances: Reseta todos os botões dinâmicos
 * - resetAllInstances: Reseta todos os botões estáticos
 */
class ClickButton {
  int pin;
  bool pressed;
  void (*click)(int pin, void* params);
  void *clickParams;

  public:
  static LDE<ClickButton*> dynamicInstances;
  static LDE<ClickButton*> allInstances;
  
  ClickButton() {
    this->pin = -1;
    this->pressed = false;
    this->clickParams = nullptr;
    this->click = nullptr;
  }

  void setup(int pin, bool isDynamic=true) {
    if(isDynamic) ClickButton::dynamicInstances.pushBack(this);
    ClickButton::allInstances.pushBack(this);
    this->pin = pin;
    this->clickParams = nullptr;
    this->click = [](int pin, void* params){
      static bool state = HIGH;
      digitalWrite(LED_BUILTIN, state);
      state = !state;
    };
    pinMode(this->pin, INPUT);
  }
  
  void checkClick() {
    if(this->pin < 0) return;

    bool state = digitalRead(this->pin);
    if(this->pressed && !state) {
      this->click(this->pin, this->clickParams);
      this->pressed = false;
    }
    else if(!this->pressed && state) {
      this->pressed = true;
    }
  }
  
  void onClick(void (*f)(int pin, void* params), void *params=nullptr) {
    this->clickParams = params;
    this->click = f;
  }

  int getPin() {
    return pin;
  }

  static void checkAllClicks() {
    ClickButton::allInstances.forEach([](ClickButton* button, void* params) {
      button->checkClick();
    });
  }

  static void resetDynamicInstances() {
    ClickButton::dynamicInstances.forEach([](ClickButton* button, void* params) {
      button->clickParams = nullptr;
      button->click = [](int pin, void* params){
        static bool state = HIGH;
        digitalWrite(LED_BUILTIN, state);
        state = !state;
      };
    });
  }

  static void resetAllInstances() {
    ClickButton::allInstances.forEach([](ClickButton* button, void* params) {
      button->clickParams = nullptr;
      button->click = [](int pin, void* params){
        static bool state = HIGH;
        digitalWrite(LED_BUILTIN, state);
        state = !state;
      };
    });
  }
};

LDE<ClickButton*> ClickButton::dynamicInstances;
LDE<ClickButton*> ClickButton::allInstances;

#endif