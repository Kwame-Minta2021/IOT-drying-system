//This codes is used to send the temperature and humidity readings of the drying crops to the blynk app 
//The connection was made through wifi manager where i had to give wifi credentials to the controller instead of hard coding it
// Blynk Template and Auth settings
#define BLYNK_TEMPLATE_ID "TMPL2wGvlVlzp"
#define BLYNK_TEMPLATE_NAME "Solar Dryer System"
#define BLYNK_AUTH_TOKEN "HoZjIhK6i6oNYh1QwCkmDIvmmdNjXlFz"


#include <Wire.h>  // Library for I2C communication
#include <LiquidCrystal_I2C.h>  // Library for LCD with I2C adapter
#include <WiFi.h>
#include <WiFiManager.h>  // WiFiManager library
#include <BlynkSimpleEsp32.h>
#include "DHT.h"



// LCD settings
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

char auth[] = BLYNK_AUTH_TOKEN;

BlynkTimer timer;
#define DHTPIN 4
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the LCD
  lcd.begin();
  lcd.backlight();

  // Initialize WiFiManager
  WiFiManager wifiManager;
  wifiManager.autoConnect("BlynkWiFiAP");

  // Connect to Blynk
  Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());

  // Initialize the DHT sensor
  dht.begin();

  // Set timer to call sendSensor function every 1000 milliseconds (1 second)
  timer.setInterval(1000L, sendSensor);
}

void sendSensor() {
  // Read humidity and temperature from the DHT sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  // Check if any reads failed
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Clear the LCD and print the new values
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%");

  // Send the data to Blynk
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);

  // Print the data to the serial monitor for debugging
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" C    Humidity: ");
  Serial.print(h);
  Serial.println(" %");
}

void loop() {
  // Run Blynk
  Blynk.run();

  // Run timer
  timer.run();
}
