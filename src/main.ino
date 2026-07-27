#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Конфігурація LCD ---
#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

// --- Піни ESP32 ---
#define LED_CRITICAL_PIN 19
#define HC_TRIG_PIN      32
#define HC_ECHO_PIN      33
#define BUZZER_PIN       18

#define OLED_SCL_PIN     22
#define OLED_SDA_PIN     21

// --- Неблокуючі таймери (millis) ---
unsigned long lastMeasureTime = 0;
const long measureInterval = 60; // Оновлення датчика кожні 60 мс

unsigned long lastBEEPTime = 0;
bool toggleState = false;

void setup() {
  pinMode(LED_CRITICAL_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(HC_TRIG_PIN, OUTPUT);
  pinMode(HC_ECHO_PIN, INPUT);

  digitalWrite(LED_CRITICAL_PIN, LOW);

  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  lcd.init();
  lcd.backlight();

  // Статичний текст виводимо один раз у setup
  lcd.setCursor(0, 0);
  lcd.print("Distance in CM:");
}

void loop() {
  unsigned long currentMillis = millis();

  // 1. Вимірювання відстані за інтервалом
  if (currentMillis - lastMeasureTime >= measureInterval) {
    lastMeasureTime = currentMillis;

    // Відправка імпульсу 10 мкс
    digitalWrite(HC_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(HC_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(HC_TRIG_PIN, LOW);

    long duration = pulseIn(HC_ECHO_PIN, HIGH, 30000); // Таймаут 30 мс
    int distanceSM = duration / 58;

    // Якщо віддзеркалення немає — вважаємо, що перешкода далі 400 см
    if (distanceSM == 0) distanceSM = 400;

    // 2. Вивід на LCD (додаємо пробіли, щоб стирати старі цифри)
    lcd.setCursor(0, 1);
    lcd.print(distanceSM);
    lcd.print(" cm    ");

    // 3. Логіка звуку та світлодіода
    const int CRITICAL_ZONE = 50;  //in sm
    const int WARN_ZONE = 200; //in sm
    const int WARM_LED_ZONE = 100;

    // 🔴 КРИТИЧНА ЗОНА
    if (distanceSM < CRITICAL_ZONE) {
      digitalWrite(LED_CRITICAL_PIN, HIGH); // LED постійно горить
      tone(BUZZER_PIN, 2000);               // Постійний звук
    }
    // 🟡 ЗОНА УВАГИ
    else if (distanceSM >= CRITICAL_ZONE && distanceSM < WARM_LED_ZONE) {
      // Динамічний інтервал: від 100 мс (біля 20 см) до 250 мс (біля 50 см)
      int interval = map(distanceSM, CRITICAL_ZONE, WARM_LED_ZONE, 100, 250);

      if (currentMillis - lastBEEPTime >= interval) {
        lastBEEPTime = currentMillis;
        toggleState = !toggleState;

        // Блимає LED і пищить Buzzer
        digitalWrite(LED_CRITICAL_PIN, toggleState ? HIGH : LOW);
        if (toggleState) tone(BUZZER_PIN, 1500);
        else noTone(BUZZER_PIN);
      }
    }
    // 🟢 ЗОНА ПОПЕРЕДЖЕННЯ
    else if (distanceSM >= WARM_LED_ZONE && distanceSM <= WARN_ZONE) {
      digitalWrite(LED_CRITICAL_PIN, LOW); // LED вимкнено

      // Динамічний інтервал: від 250 мс (біля 50 см) до 600 мс (біля 100 см)
      int interval = map(distanceSM, WARM_LED_ZONE, WARN_ZONE, 250, 600);

      if (currentMillis - lastBEEPTime >= interval) {
        lastBEEPTime = currentMillis;
        toggleState = !toggleState;

        if (toggleState) tone(BUZZER_PIN, 1000);
        else noTone(BUZZER_PIN);
      }
    }
    // ⚪ БЕЗПЕЧНА ЗОНА
    else {
      digitalWrite(LED_CRITICAL_PIN, LOW);
      noTone(BUZZER_PIN);
    }
  }
}