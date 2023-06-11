#include "src/temp/libs/Thermistor/Thermistor.h"
#include "src/temp/libs/HeatingElement/HeatingElement.h"
#include "src/temp/libs/Button/Button.h"

#include "LiquidCrystal_I2C.h"
#include "Encoder.h"

#include "Board.h"
#include "MainModule.h"

Thermistor thermistor;
HeatingElement heater;
Encoder encoder(ENC_B_PIN,ENC_A_PIN);
Button button;
Button enc_button;
LiquidCrystal_I2C lcd(LCD_ADDR,LCD_COLS,LCD_ROWS);

uint16_t current_temp=0;
uint16_t set_temp=DEFAULT_TEMP;

uint32_t display_tick=0;
uint32_t temp_poll_tick=0;

long encoder_pos = -999;
long new_encoder_pos = 0;

byte thermometer[8] = //icon for termometer
{
    B00100,
    B01010,
    B01010,
    B01110,
    B01110,
    B11111,
    B11111,
    B01110
};

byte degree[] = {
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

void setup()
{
    Serial.begin(SERIAL_BAUDRATE);

    thermistor.init(THERMISTOR_PIN,FIXED_RESISTOR_OHMS,THERMISTOR_GRADIENT,THERMISTOR_OFFSET);

    heater.init(HEATER_PIN);
    heater.setTarget(DEFAULT_TEMP);
    heater.setPIDconsts(HEATER_PID_P,HEATER_PID_I,HEATER_PID_D,HEATER_MIN_DUTY,HEATER_MAX_DUTY);

    button.init(BUTTON_PIN);
    enc_button.init(ENC_SW_PIN);

    lcd.init();
    lcd.backlight();

    lcd.createChar(1,thermometer);
    lcd.createChar(2,degree);
    
    drawBootPage();
    delay(1000);
    lcd.clear();
}

void loop()
{
    if (millis()-temp_poll_tick>TEMP_POLL_INT_MS)
    {
        current_temp = thermistor.getTemperature();
        heater.runPID(current_temp);
        temp_poll_tick = millis();
    }
    
    heater.setTarget(set_temp);
    
    if (millis()-display_tick>LCD_REFRESH_INT_MS)
    {
        drawMainPage();
        display_tick = millis();
    }

    new_encoder_pos = encoder.read()/2;   
    
    if (new_encoder_pos>encoder_pos){set_temp += TEMP_INC;}
    else if (new_encoder_pos<encoder_pos){set_temp -= TEMP_INC;}

    if (set_temp>MAX_TEMP){set_temp = MAX_TEMP;}
    if (set_temp<MIN_TEMP){set_temp = MIN_TEMP;}

    encoder_pos = new_encoder_pos;
}

void drawBootPage()
{
    lcd.setCursor(4,0);
    lcd.print("HAKKO 907");
    lcd.setCursor(1,1);
    lcd.print("SOLDER STATION");
}

void drawMainPage()
{
    lcd.setCursor(4,0);
    lcd.write(1);
    lcd.setCursor(6,0);
    
    if (current_temp<100)
        lcd.print("0");

    lcd.print(current_temp);
    lcd.setCursor(9,0);
    lcd.write(2);
    lcd.setCursor(10,0);
    lcd.print("C");

    lcd.setCursor(4,1);
    lcd.print("S");
    lcd.setCursor(6,1);

       if (set_temp<100)
        lcd.print("0");

    lcd.print(set_temp);
    lcd.setCursor(9,1);
    lcd.write(2);
    lcd.setCursor(10,1);
    lcd.print("C");
}