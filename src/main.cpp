#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

#define LED_CRITICAL_PIN 19
#define HC_TRIG_PIN 32
#define HC_ECHO_PIN 33

#define BUZZER_PIN 18
#define OLED_SCL_PIN  22
#define OLED_SDA_PIN  21


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64



void setup() {


  pinMode(LED_CRITICAL_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(HC_TRIG_PIN, OUTPUT);
  pinMode(HC_ECHO_PIN, INPUT);
digitalWrite(LED_CRITICAL_PIN, LOW);

Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN );
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Parktronic:");
  delay(100);
  lcd.clear();
}

void loop() {

 digitalWrite(HC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(HC_TRIG_PIN, LOW);

  // Read the result:
   static unsigned int distance = 10;
   unsigned int distanceSM = 0 ;
   distance = pulseIn(HC_ECHO_PIN, HIGH);
distanceSM = distance / 58;
const int CRITICAL_DISANCE_SM = 50;
const int WARN_DISANCE_SM = 20;

lcd.setCursor(0, 0);
lcd.print("Distance in CM:");
lcd.setCursor(1, 1);
lcd.print( distanceSM);

if (distanceSM < CRITICAL_DISANCE_SM){
 tone(BUZZER_PIN, 262, 250);
  digitalWrite(LED_CRITICAL_PIN, LOW);
} else if (distance >= CRITICAL_DISANCE_SM && distance < WARN_DISANCE_SM) {

}



}
