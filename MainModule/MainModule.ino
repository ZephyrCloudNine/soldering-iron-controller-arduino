#include "src/temp/libs/Thermistor/Thermistor.h"
#include "src/temp/libs/HeatingElement/HeatingElement.h"
#include "src/temp/libs/Button/Button.h"

#include "Encoder.h"

#include "Board.h"
#include "MainModule.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(LCD_COLS, LCD_ROWS, &Wire, -1);

Thermistor thermistor;
HeatingElement heater;
Encoder *encoder;
Button encoder_button;

float current_temp=0;
uint16_t set_temp=DEFAULT_TEMP;

uint32_t display_tick=0;
uint32_t temp_poll_tick=0;
uint32_t debug_poll_tick=0;

long encoder_pos = -999;
long new_encoder_pos = 0;

enum controller_state
{
  off,
  heating,
  cooling,
  ready
};

controller_state status = off;

void setup()
{
    if( !display.begin(SSD1306_SWITCHCAPVCC, LCD_ADDR))
    { 
        Serial.println(F("Display error! Please check connections or replace LCD module."));
        for(;;);
    }

    display.clearDisplay();
    drawBootPage();
    display.display();
    delay(1000);

    Serial.begin(SERIAL_BAUDRATE);
    thermistor.init(THERMISTOR_PIN,FIXED_RESISTOR_OHMS,THERMISTOR_GRADIENT,THERMISTOR_OFFSET);

    heater.init(HEATER_PIN);
    heater.setTarget(DEFAULT_TEMP);
    heater.setPIDconsts(HEATER_PID_P,HEATER_PID_I,HEATER_PID_D,HEATER_MIN_DUTY,HEATER_MAX_DUTY);
    heater.drive(0);

    encoder = new Encoder(ENC_A_PIN,ENC_B_PIN);
    encoder_button.init(ENC_SW_PIN);
    
}

void loop()
{   
    controllerFSM();
    handleEncoder();
  
    if (millis()-temp_poll_tick>TEMP_POLL_INT_MS) 
    {
      current_temp = thermistor.getTemperature();
      
      if (status!=off)
      {
        heater.setTarget(set_temp);
        heater.runPID(current_temp);
      }
    
      temp_poll_tick = millis();
    }
    
    if (millis()-display_tick>LCD_REFRESH_INT_MS)
    {
      drawMainPage();
      display_tick = millis();
    }

    #if DEBUG==1
      if (millis()-debug_poll_tick>DEBUG_INTERVAL_MS)
      {
        Serial.print("Set temp(C) ");
        Serial.print(set_temp);
        Serial.print(" , ");
        Serial.print("Current temp(C) ");
        Serial.print(current_temp);
        Serial.print(" , ");
        Serial.print("Heater duty ");
        Serial.print(heater.getDuty());
        Serial.println();

        debug_poll_tick = millis();
      }
    #endif 

}

void drawBootPage()
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(45,20);
  display.print("WONKY");
  display.setCursor(7,40);
  display.print("SOLDER STATION v1.0");
}

void drawMainPage()
{   
    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(6,6);
    display.print("SET");
    display.setCursor(28,6);

      if (set_temp<100)
        display.print("0");

    display.print(set_temp,0);
    display.setCursor(50,6);
    display.print("C");

    display.setCursor(95,6);

    switch (status)
    {
      case off:
        display.print("OFF");
        break;

      case heating:
        display.print("HEAT");
        break;

      case cooling:
        display.print("COOL");
        break;

      case ready:
        display.print("RDY");
        break;
    }
   
    display.setTextSize(3);
    display.setCursor(26,30);
    
    if (current_temp<100)
        display.print("0");

    display.print(current_temp,0);
    display.setCursor(91,30);
    display.print("C");
    display.display();
}

void controllerFSM()
{
  if (encoder_button.isShortPressed() && status == off)
  {
    status = ready;
  } 
  else if (encoder_button.isHeld())
  {
    status = off;
    heater.drive(0);
  }
  else if (status!=off)
  {
    if (current_temp > set_temp + TEMP_HYSTERSIS)
    {
      status = cooling;
    }
    else if (current_temp < set_temp -  TEMP_HYSTERSIS)
    {
      status = heating;
    }
    else
    {
      status = ready;
    }    
  }
}

void handleEncoder()
{
  new_encoder_pos = encoder->read()/2;
  
  if (new_encoder_pos>encoder_pos){set_temp += TEMP_INC;}
  else if (new_encoder_pos<encoder_pos){set_temp -= TEMP_INC;}

  if (set_temp>MAX_TEMP){set_temp = MAX_TEMP;}
  if (set_temp<MIN_TEMP){set_temp = MIN_TEMP;}

  encoder_pos = new_encoder_pos;
}