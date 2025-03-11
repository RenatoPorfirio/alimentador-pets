// #include <Wire.h>
// #include <RTClib.h>
// #include <EEPROM.h>
// #include <LiquidCrystal_I2C.h>

// LiquidCrystal_I2C lcd(0x27,20,4);
// RTC_DS1307 rtc;

// #define TIME_REG_LIMIT 3

// #define MENU_CONTEXT 0
// #define ADD_HOUR_CONTEXT 1
// #define REMOVE_HOUR_CONTEXT 2
// #define DISPLAY_TIME_CONTEXT 3
// #define CLOCK_CONFIG_CONTEXT 4

// #define SCREEN_ON_LIGHT 0
// #define SCREEN_ON 1
// #define SCREEN_OFF 2

// // estruturas ---------------------------------------------------------
// typedef struct
// {
//   int8_t index;
//   int8_t hour;
//   int8_t minute;
//   int8_t second;
// } TimeData;

// TimeData nulltime = {-1, -1, -1, -1};

// class TimeManager
// {
//   TimeData reg[TIME_REG_LIMIT];
//   uint8_t regQnt = 0;
//   public:
//   TimeManager()
//   {
//     uint8_t timeQnt = EEPROM.read(0);
//     Serial.println(timeQnt);
//     if (timeQnt > TIME_REG_LIMIT) {
//       timeQnt = 0;
//       TimeManager::memClear();
//     }
//     for (uint8_t i = 0; i < timeQnt; i++) {
//       TimeData time;
//       EEPROM.get(1 + i * sizeof(TimeData), time);
//       reg[i] = time;
//     }
//   }
//   static void memClear()
//   {
//     Serial.println("clear");
//     EEPROM.write(0, 0);
//   }

//   void addTimeData(int8_t hour, int8_t minute, int8_t second)
//   {
//     if(regQnt == TIME_REG_LIMIT) return;
//     TimeData time = {regQnt, hour, minute, second};
//     reg[regQnt] = time;
//     EEPROM.put(1 + regQnt * sizeof(TimeData), time);
//     EEPROM.write(0, ++regQnt);
//     Serial.println(EEPROM.read(0));
//   }

//   void removeTimeData(int8_t index)
//   {
//     if (regQnt == 0) return;
//     TimeData target = reg[index];
//     regQnt--;

//     for (int8_t i = index; i < regQnt; i++) {
//       reg[i] = reg[i+1];
//       reg[i].index = i;
//       EEPROM.put(1 + i * sizeof(TimeData), reg[i]);
//     }
//     EEPROM.write(0, regQnt);
//   }

//   TimeData* getTimeReg()
//   {
//     return reg;
//   }
//   uint8_t getTimeRegQnt()
//   {
//     return regQnt;
//   }
// };

// class Button
// {
//   int pin;
//   bool pressed;
//   void (*click)(int pin, void* params);
//   void *clickParams;
  
//   public:
  
//   Button(int pin)
//   {
//     this->pin = pin;
//     this->pressed = false;
//     this->clickParams = nullptr;
//     this->click = [](int pin, void* params){
//       static bool state = HIGH;
//       digitalWrite(LED_BUILTIN, state);
//       state = !state;
//     };
//     pinMode(this->pin, INPUT);
//   }
  
//   void checkClick()
//   {
//     if(this->pin < 0) return;

//     bool state = digitalRead(this->pin);
//     if(this->pressed && !state) {
//       this->click(this->pin, this->clickParams);
//       this->pressed = false;
//     } 
//     else if(!this->pressed && state) {
//       this->pressed = true;
//     }
//   }
  
//   void onClick(void (*f)(int pin, void* params), void *params=nullptr)
//   {
//     this->clickParams = params;
//     this->click = f;
//   }
// };

// typedef struct {
//   int current;
//   int currentOption;
//   int optionsQnt;
//   bool shouldScreenUpdate;
//   bool displayOn;
// } Context;

// // definições -------------------------------------------------------------
// Button* upButton;
// Button* downButton;
// Button* backButton;
// Button* forwardButton;
// Button* menuButton;
// Button* displayButton;
// Button* buttons[6];

// void resetDynamicButtons();

// Context context;
// TimeManager* timeManager;

// String timeToString(TimeData time);
// TimeData dateTimeToTimeData(DateTime &datetime);
// void setStaticButtonHandlers();
// void setDynamicButtonHandlers();
// void handleContext();
// void handleInputs();

// void renderMenuState();
// void renderMenuOption();

// void renderAddHourState();

// // main -------------------------------------------------------------------
// void setup()
// {
//   Wire.begin();
//   Serial.begin(9600);
  
//   upButton = new Button(8);
//   downButton = new Button(7);
//   backButton = new Button(6);
//   forwardButton = new Button(5);
//   menuButton = new Button(4);
//   displayButton = new Button(3);
  
//   timeManager = new TimeManager();

//   buttons[0] = upButton;
//   buttons[1] = downButton;
//   buttons[2] = backButton;
//   buttons[3] = forwardButton;
//   buttons[4] = menuButton;
//   buttons[5] = displayButton;
  
//   setStaticButtonHandlers();

//   int8_t x = EEPROM.read(0);
//   Serial.println(x);
//   Serial.println("Searching for I2C devices...");
  
//   for (byte i = 1; i < 127; i++) {
//     Wire.beginTransmission(i);
//     if (Wire.endTransmission() == 0) {
//       Serial.print("Device found at address: 0x");
//       Serial.println(i, HEX);
//       delay(10);
//     }
//   }

//    if (!rtc.begin()) {
//     Serial.println("Couldn't find RTC");
//     Serial.flush();
//     while (1) delay(10);
//   }

//   if (!rtc.isrunning()) {
//     Serial.println("RTC is NOT running, let's set the time!");
//     rtc.adjust(DateTime(2024, 1, 1, 0, 0, 0));
//   }
  
//   lcd.init();
//   lcd.backlight();
  
//   context.current = MENU_CONTEXT;
//   context.currentOption = 0;
//   context.optionsQnt = 3;
//   context.shouldScreenUpdate = true;
//   context.displayOn = true;
// }


// void loop()
// {
//   handleContext();
//   handleInputs();
// }

// // implementações gerais -------------------------------------------------
// void handleContext()
// {
//   switch(context.current) {
//     case MENU_CONTEXT:
//         if(context.shouldScreenUpdate) {
//         context.currentOption = 0;
//         context.optionsQnt = 3;
//         renderMenuState();
//         setDynamicButtonHandlers();
//         context.shouldScreenUpdate = false;
//         Serial.println("screen updated on menu context.");
//         }
//     	break;
//     case ADD_HOUR_CONTEXT:
//     	if(context.shouldScreenUpdate) {
//         renderAddHourState();
//         setDynamicButtonHandlers();
//         context.shouldScreenUpdate = false;
//         Serial.println("screen updated on add hour context.");
//         }
//     	break;
//     case REMOVE_HOUR_CONTEXT:
//     	if(context.shouldScreenUpdate) {
//         renderRemHourState();
//         setDynamicButtonHandlers();
//         context.shouldScreenUpdate = false;
//         Serial.println("screen updated on rem hour context.");
//       }
//     	break;
//     case CLOCK_CONFIG_CONTEXT:
//     	if(context.shouldScreenUpdate) {
//         renderConfigClockState();
//         setDynamicButtonHandlers();
//         context.shouldScreenUpdate = false;
//         Serial.println("screen updated on clock config context.");
//       }
//     	break;
//     case DISPLAY_TIME_CONTEXT:
//       renderDisplayClockState();
//     	if(context.shouldScreenUpdate) {
//         setDynamicButtonHandlers();
//         DateTime now = rtc.now();
//         lcd.clear();
//         lcd.setCursor(0, 0);
//         lcd.print(timeToString(dateTimeToTimeData(now)));
//         context.shouldScreenUpdate = false;
//         Serial.println("screen updated on display time context.");
//       }
//     	break;
//     default:
//     	context.current = MENU_CONTEXT;
//   }
// }

// void setDynamicButtonHandlers()
// {
//   resetDynamicButtons();
  
//   switch(context.current) {
//     case MENU_CONTEXT:
//     	upButton->onClick([](int pin, void* params){
//           context.currentOption -= 1;
//           if (context.currentOption < 0) {
//             context.currentOption = context.optionsQnt - 1;
//           }
//           renderMenuOption();
//         });
//     	downButton->onClick([](int pin, void* params){
//           context.currentOption = (context.currentOption + 1) % context.optionsQnt;
//           renderMenuOption();
//         });
//       backButton->onClick([](int pin, void* params){
//           context.current = DISPLAY_TIME_CONTEXT;
//           context.shouldScreenUpdate = true;
//         });
//     	forwardButton->onClick([](int pin, void* params){
//           switch (context.currentOption) {
//             case 0:
//             	context.current = ADD_HOUR_CONTEXT;
//             	break;
//             case 1:
//             	context.current = REMOVE_HOUR_CONTEXT;
//             	break;
//             case 2:
//               context.current = CLOCK_CONFIG_CONTEXT;  
// 			      default:
//             	lcd.clear();
//             	lcd.setCursor(0,0);
//             	lcd.print("Opcao invalida!");
//           }
//           context.shouldScreenUpdate = true;
//         });
//     	break;
//     case ADD_HOUR_CONTEXT:
//       forwardButton->onClick([](int pin, void* params){
//           DateTime datetime(2025,3,8,23,59,0);
//           timeManager->addTimeData(datetime.hour(), datetime.minute(), datetime.second());
//           context.current = DISPLAY_TIME_CONTEXT;
//           context.shouldScreenUpdate = true;
//         });
//     	break;
//     default:
//     	break;
//   }
// }

// void setStaticButtonHandlers()
// {
//   displayButton->onClick([](int pin, void *params) {
//     static int lcdState = SCREEN_ON_LIGHT;
//     lcdState = (lcdState + 1) % 3;
//     switch (lcdState) {
//       case SCREEN_ON_LIGHT:
//       	lcd.backlight();
//       	lcd.display();
//         context.displayOn = true;
//       	context.shouldScreenUpdate = true;
//       	break;
//       case SCREEN_ON:
//       	lcd.noBacklight();
//       	break;
//       case SCREEN_OFF:
//       	lcd.noDisplay();
//         context.displayOn = false;
//       	break;
//     }
//   });
//   menuButton->onClick([](int pin, void *params) {
//     context.current = context.current == MENU_CONTEXT ? DISPLAY_TIME_CONTEXT : MENU_CONTEXT;
//     context.shouldScreenUpdate = true;
//   });
// }

// void resetDynamicButtons()
// {
//   for(int i = 0; i < 4; i++) {
//     buttons[i]->onClick([](int pin, void* params){
//       static bool state = HIGH;
//       digitalWrite(LED_BUILTIN, state);
//       state = !state;
//     });
//   }
// }

// void handleInputs()
// {
//   for(Button* button : buttons) {
//     button->checkClick();
//   }
// }

// String timeToString(TimeData time)
// {
//   String str = (time.hour < 10 ? "0" : "");
//   str.concat(String(time.hour, DEC));
//   str.concat(":");
//   str.concat((time.minute < 10 ? "0" : ""));
//   str.concat(String(time.minute, DEC));
//   str.concat(":");
//   str.concat((time.second < 10 ? "0" : ""));
//   str.concat(String(time.second, DEC));
//   return str;
// }

// TimeData dateTimeToTimeData(DateTime& datetime)
// {
//   TimeData time = {
//     datetime.hour(),
//     datetime.minute(),
//     datetime.second(),
//   };
//   return time;
// }

// // implementações menu -------------------------------------------------
// void renderMenuState()
// {
//   if (!context.displayOn) return;

//   lcd.clear();
  
//   static String menuOptions[] = {
//     "adicionar hora",
//     "remover hora",
//     "conf. Relogio"
//   };
  
//   for(int i = 0; i < context.optionsQnt; i++) {
//     lcd.setCursor(0, i);
//     lcd.print(menuOptions[i]);
//   }
  
//   renderMenuOption();
// }

// void renderMenuOption()
// {
//   if (!context.displayOn) return;
  
//   for(int i = 0; i < context.optionsQnt; i++) {
//   	lcd.setCursor(15, i);
//     lcd.print(" ");
//   }
//   lcd.setCursor(15, context.currentOption);
//   lcd.print("<");
// }

// // implementações addHour -------------------------------------------------
// void renderAddHourState()
// {
//   if (!context.displayOn) return;
  
//   lcd.clear();
//   lcd.setCursor(0,0);
//   lcd.print("TELA ADDHOUR");
// }

// // implementações remHour -------------------------------------------------
// void renderRemHourState()
// {
//   if (!context.displayOn) return;
  
//   TimeData* time = timeManager->getTimeReg();
//   uint8_t length = timeManager->getTimeRegQnt();

//   lcd.clear();
//   for (uint8_t i = 0; i < length; i++) {
//     lcd.setCursor(0, i);
//     lcd.print(timeToString(time[i]));
//   }
// }

// // implementações configClock -------------------------------------------------
// void renderConfigClockState()
// {
//   if (!context.displayOn) return;
  
//   lcd.clear();
//   lcd.setCursor(0,0);
//   lcd.print("TELA CONFIG");
// }

// // implementações displayClock -------------------------------------------------
// void renderDisplayClockState()
// {
//   if (!context.displayOn) return;
  
//   static double lastTime = 0;
//   double currentTime = millis();
//   DateTime now = rtc.now();
  
//   if (currentTime - lastTime > 1000) {
//     lcd.setCursor(0, 0);
//     lcd.print(timeToString(dateTimeToTimeData(now)));
//     lastTime = currentTime;
//   }
// }