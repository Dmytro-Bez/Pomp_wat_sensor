#include<Arduino.h>
#include <Adafruit_GFX.h>					    //Connect lib.work LCD
#include <Adafruit_PCD8544.h>					//Connect lib.work LCD NOKIA 5110

#define D8 18                                                           //Serial clock out (SCLK)
#define D7 23                                                           //Serial data out (DIN)
#define D6 19                                                           //Data/Command select (D/C)
#define D5 5                                                            //LCD chip select (CS)
#define D2 14                                                           //LCD reset (RST)

Adafruit_PCD8544 display = Adafruit_PCD8544(D8, D7, D6, D5, D2);            //Protot. object display LCD

static void start_config();
static void pr_test_nokia();

void setup() {
  Serial.begin(115200);
  start_config();
}

void loop() {
 
}

static void start_config() {
  Serial.println("Start config mode");                    //Add function setting 
  
  display.begin();                                        //Init display
  display.setContrast(70);                                //Contrast
  display.clearDisplay();                                 //Clear display
  pr_test_nokia();                                               //Testing win
}

static void pr_test_nokia() {
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(15,0);
  display.println("TEST_MENU:");
  display.display();
}
