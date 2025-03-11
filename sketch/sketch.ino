#include "display-control.h"
#include "button-config.h"
#include "LDE.h"

#include <EEPROM.h>

DisplayControl display;

void setup() {
  display.begin();
  setupButtons();

  LDE<int>* list = new LDE<int>();
  list->pushBack(1);
  list->pushBack(2);
  list->pushBack(3);
  list->pushBack(4);
  displayButton.onClick([](int pin, void* params){
    LDE<int> *list = (LDE<int>*) params;
    display.print(String(list->getIndex(5, [](int a, int b) {
      return a == b;
    }), 10));
  }, (void*)list);
}

void loop() {
  buttons.forEach([](ClickButton* button) {
    button->checkClick();
  });
}