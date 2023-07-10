#include <USI_TWI_Master.h>
#include <TinyWireM.h>
#include <TinyLiquidCrystal_I2C.h>
#include <dht.h>
#define DHTPIN 1
dht DHT;
int temperature = 0;
int humidity = 0;
TinyLiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  TinyWireM.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.setCursor(0,1);
  lcd.print("Humidity:");
}

void loop() {
  int chk = DHT.read11(DHTPIN);
  temperature = (DHT.temperature);
  humidity = (DHT.humidity);

  lcd.setCursor(10,0);
  lcd.print(temperature); lcd.print("*C");
  lcd.setCursor(10,1);
  lcd.print(humidity); lcd.print("%");
  delay(2000);
}