#include "src/temp/libs/Thermistor/Thermistor.h"
#include "src/temp/libs/HeatingElement/HeatingElement.h"
#include "src/temp/libs/Button/Button.h"

#include "LiquidCrystal_I2C.h"
#include "Encoder.h"

#include "Board.h"
#include "MainModule.h"

Thermistor thermistor;
HeatingElement heater;
Encoder encoder(ENC_A_PIN,ENC_B_PIN);
Button button;
Button enc_button;
LiquidCrystal_I2C lcd(LCD_ADDR,LCD_COLS,LCD_ROWS);

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
    
    drawBootPage();
    delay(1000);
    lcd.clear();
}

void loop()
{
  
}

void drawBootPage()
{
    lcd.setCursor(4,0);
    lcd.print("HAKKO 907");
    lcd.setCursor(1,1);
    lcd.print("SOLDER STATION");
}