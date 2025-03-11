#ifndef _CLICK_BUTTON_H_
#define _CLICK_BUTTON_H_

class ClickButton {
  int pin;
  bool pressed;
  void (*click)(int pin, void* params);
  void *clickParams;
  
  public:
  
  ClickButton() {
    this->pin = -1;
    this->pressed = false;
    this->clickParams = nullptr;
    this->click = nullptr;
  }

  void setup(int pin) {
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
};

#endif