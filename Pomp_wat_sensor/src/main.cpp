#include<Arduino.h>
#include <Adafruit_GFX.h>					                                      //Connect lib.work LCD
#include <Adafruit_PCD8544.h>					                                  //Connect lib.work LCD NOKIA 5110

#define D8 18                                                           //Serial clock out (SCLK)
#define D7 23                                                           //Serial data out (DIN)
#define D6 19                                                           //Data/Command select (D/C)
#define D5 5                                                            //LCD chip select (CS)
#define D2 14                                                           //LCD reset (RST)
#define DL 27
#define BUTT_1 2
#define BUTT_2 4
#define BUTT_E 15
#define PIN_POMP  16  //pin detect pomp
#define INFO_LED_PIN 17                                                                 //led pin
/*-----------------------------------------------------------------*/
#define ERROR_CODE_SIZE 4
#define ERROR_CODE_LONG 0
#define ERROR_CODE_SHORT 1 

int MOTOR_ERROR_CODE[ERROR_CODE_SIZE] = {ERROR_CODE_SHORT, ERROR_CODE_SHORT,  ERROR_CODE_LONG, ERROR_CODE_LONG};
int SENSOR_ERROR_CODE[ERROR_CODE_SIZE] = {ERROR_CODE_SHORT, ERROR_CODE_SHORT, ERROR_CODE_SHORT, ERROR_CODE_LONG};
int WATER_ERROR_CODE[ERROR_CODE_SIZE] = {ERROR_CODE_SHORT, ERROR_CODE_SHORT, ERROR_CODE_SHORT, ERROR_CODE_SHORT};
typedef enum {
  MOTOR_ERROR,                               //error EEPROM memory blinking code is ..--
  SENSOR_ERROR,                              //start data transfer  blinking code is ...-
  WATER_ERROR,                               //start data transfer  blinking code is ....
  } app_status;
app_status a_status;
/*-----------------------------------------------------------------*/
void drawMenu();
void checkIfDownButtonIsPressed();
void checkIfUpButtonIsPressed();
void checkIfSelectButtonIsPressed();
void setContrast();
void turnBacklightOn();
void turnBacklightOff();
void resetDefaults();
static void pomp_g();
static void f_cactus_pop();
static void f_error();
static void info_blink(int code_array[]);

boolean backlight = true;
int contrast=50;
int menuitem = 1;
int page = 1;
volatile boolean up = false;
volatile boolean down = false;
volatile boolean middle = false;
int downButtonState = 0;
int upButtonState = 0;  
int selectButtonState = 0;          
int lastDownButtonState = 0;
int lastSelectButtonState = 0;
int lastUpButtonState = 0;
Adafruit_PCD8544 display = Adafruit_PCD8544(D8, D7, D6, D5, D2);

void setup() {
  pinMode(PIN_POMP, OUTPUT);
  pinMode(BUTT_1, INPUT_PULLUP);
  pinMode(BUTT_2, INPUT_PULLUP);
  pinMode(BUTT_E, INPUT_PULLUP);
  pinMode(DL,OUTPUT);

  digitalWrite(DL,LOW); //Turn Backlight ON
  digitalWrite(PIN_POMP,LOW);

  Serial.begin(115200);
  
  Serial.println("Go work");
  display.begin();      
  display.setContrast(contrast); //Set contrast to 50
  display.clearDisplay(); 
  display.display();   
}

void loop() {
  drawMenu();
  downButtonState = digitalRead(BUTT_1);
  selectButtonState = digitalRead(BUTT_E);
  upButtonState =   digitalRead(BUTT_2);
 
  checkIfDownButtonIsPressed();
  checkIfUpButtonIsPressed();
  checkIfSelectButtonIsPressed();

  if (up && page == 1 ) {
    up = false;
    menuitem--;
    if (menuitem==0){
      menuitem=3;
    }      
  } else if (up && page == 2 ) {
    up = false;
    contrast--;
    setContrast();
  }
  if (down && page == 1) {
    down = false;
    menuitem++;
    if (menuitem==4){
      menuitem=1;
    }      
    } else if (down && page == 2 ) {
      down = false;
      contrast++;
      setContrast();
    }
    if (middle) {
    middle = false;
    if (page == 1 && menuitem==2) {
      if (backlight) {
        backlight = false;
        turnBacklightOff();
        } else {
        backlight = true; 
        turnBacklightOn();
       }
    }
    if(page == 1 && menuitem ==3){
      resetDefaults();
    }else if (page == 1 && menuitem==1) {
          page=2;
    } else if (page == 2) {
          page=1;
    }
  }
}

void checkIfDownButtonIsPressed(){
    if (downButtonState != lastDownButtonState) {
      if (downButtonState == 0) {
        down=true;
      }
      delay(50);
    }
   lastDownButtonState = downButtonState;
}

void checkIfUpButtonIsPressed(){
  if (upButtonState != lastUpButtonState) {
    if (upButtonState == 0) {
      up=true;
    }
    delay(50);
  }
   lastUpButtonState = upButtonState;
}

void checkIfSelectButtonIsPressed(){
   if (selectButtonState != lastSelectButtonState) {
    if (selectButtonState == 0) {
      middle=true;
    }
    delay(50);
  }
   lastSelectButtonState = selectButtonState;
}
 
void drawMenu(){
    if (page==1) {    
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("MAIN MENU");
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(0, 15);
    if (menuitem==1) { 
      display.setTextColor(WHITE, BLACK);
    } else {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Kaktus");
    display.setCursor(0, 25);
    if (menuitem==2) {
      display.setTextColor(WHITE, BLACK);
    } else {
      display.setTextColor(BLACK, WHITE);
    }    
    display.print(">Light: ");
    if (backlight) {
      display.print("ON");
    } else {
      display.print("OFF");
    }
    display.display();
    
    if (menuitem==3) { 
      display.setTextColor(WHITE, BLACK);
    } else {
      display.setTextColor(BLACK, WHITE);
    }  
    display.setCursor(0, 35);
    display.print(">Reset");
    display.display();
  } else if (page==2) {
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print("CONTRAST");
    display.drawFastHLine(0,10,83,BLACK);
    display.setCursor(5, 15);
    display.print("PUSH");
    display.setTextSize(2);
    display.setCursor(5, 25);
    // display.print(contrast);
    f_cactus_pop();
 
    display.setTextSize(2);
    display.display();
  }
}

void resetDefaults(){
    contrast = 50;
    setContrast();
    backlight = true;
    turnBacklightOn();
}

void setContrast(){
    display.setContrast(contrast);
    display.display();
}

void turnBacklightOn(){
    digitalWrite(DL,LOW);
}

void turnBacklightOff(){
    digitalWrite(DL,HIGH);
}

static void pomp_g(){
  digitalWrite(PIN_POMP, HIGH);
}

static void f_cactus_pop(){
  bool button_s = false;
  if(BUTT_E == false){
    a_status = MOTOR_ERROR;
    //back to menu
    // Serial.println("Kaktus work");
  } else {
    pomp_g(); //work to the pomp
    Serial.println("Kaktus work");
  }
}

static void f_error(){
  switch(a_status){
    case MOTOR_ERROR:
      info_blink(MOTOR_ERROR_CODE);
      break;
    case SENSOR_ERROR:
      info_blink(SENSOR_ERROR_CODE);
      break;
    case WATER_ERROR:
      info_blink(WATER_ERROR_CODE);
      break;
    default:
      break;
  }
}

static void info_blink(int code_array[]) {
  for (int i=0; i<ERROR_CODE_SIZE; i++) {
    int code = code_array[i];
    digitalWrite(INFO_LED_PIN, HIGH);                                                     // turn the LED on (HIGH is the voltage level)
    delay(code == ERROR_CODE_LONG?1000:500);                                              // wait for a second
    digitalWrite(INFO_LED_PIN, LOW);                                                      // turn the LED off by making the voltage LOW
    delay(500); 
  }
  delay(2000); 
}
