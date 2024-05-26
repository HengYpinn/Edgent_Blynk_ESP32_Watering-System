// *** MAIN SETTINGS ***
// Replace this block with correct template settings.
// You can find it for every template here:
//
//   https://blynk.cloud/dashboard/templates

#define BLYNK_TEMPLATE_ID "<Insert your BLYNK_TEMPLATE_ID>"
#define BLYNK_TEMPLATE_NAME "Plant watering system"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

#include "BlynkEdgent.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>

#define sensor 33
#define relay 4
// Define the DHT22 data pin
#define DHTPIN 5   
#define DHTTYPE DHT22
// Create a DHT22 object
DHT dht(DHTPIN, DHTTYPE);

// Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

BlynkTimer timer;

// Get the ultrasonic sensor values
void soilMoisture() {
  float value = analogRead(sensor);
  value = (float)(value - 0) * (100 - 0) / (4095 - 0) + 0;
  value = 100 - value;
  Blynk.virtualWrite(V0, value);
  Serial.println(value);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Moisture :");
  lcd.print(value);
  lcd.print(" ");
  delay(1000);  
}

// Get the button value
BLYNK_WRITE(V1) {
  bool Relay = param.asInt();
  if (Relay == 1) {
    digitalWrite(relay, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Motor is ON ");
  } else {
    digitalWrite(relay, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Motor is OFF");
}
}

void humidityTemp() {
  float humidity = dht.readHumidity();
  float temperatureC = dht.readTemperature(); 
  float temperatureF = dht.readTemperature(true); // Get temp in Fahrenheit
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, temperatureC);
  Blynk.virtualWrite(V4, temperatureF);

  // Check for errors
  if (isnan(humidity) || isnan(temperatureC)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Display the results
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humidity :");
  lcd.print(humidity);
  lcd.print("% ");
  delay(1000);

  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.print(" *C ");
  Serial.print(temperatureF);
  Serial.println(" *F");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature :");
  lcd.setCursor(0, 1);
  lcd.print(temperatureC);
  lcd.print("C ");
  delay(1000);
}

void setup()
{
// Debug console. Make sure you have the same baud rate selected in your serial monitor
  Serial.begin(115200);
  delay(100);
  BlynkEdgent.begin();
  dht.begin();  
  lcd.init();
  lcd.backlight();
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);

  lcd.setCursor(1, 0);
  lcd.print("System Loading");
  for (int a = 0; a <= 15; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(200);
  }
  lcd.clear();
}

void loop() {
  BlynkEdgent.run();
  soilMoisture();
  humidityTemp();
  delay(300);
}

