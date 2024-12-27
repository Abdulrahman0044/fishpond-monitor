#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <Wire.h>
#include <ESPAsyncWebServer.h>
#include <SimpleTimer.h>
#include <LiquidCrystal_I2C.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
#include <HTTPClient.h>
// Define the LCD parameters
#define LCD_ADDRESS 0x27 // I2C address of the LCD
#define LCD_COLUMNS 20   // Number of columns
#define LCD_ROWS 4      // Number of rows

SimpleTimer timer;

float calibration_factor = 2.73;
unsigned long wifiTimeout = 10000;  // 10 seconds timeout

// Data wire is connected to GPIO 4 on the ESP32
#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// WiFi credentials
const char* ssid = " WIFI_SSID ";
const char* password = "WIFI_PASSWORD";

// Sensor pins
const int pH_pin = 34;
const int DO_pin = 35;
//const int NH3_pin = 36;

float temperature = 0.0;
float voltage = 0;         
float pHValue = 0;

float ph_act;
float DOValue = 0.00;
float TDS_Sensor = 0.00;
// int NH3Value = 0;

hd44780_I2Cexp lcd; // declare lcd object: auto locate & auto config expander chip
hd44780_I2Cexp lcd2(0x27);


WiFiClient client;
String data;

void setup() {
  Serial.begin(115200);
  sensors.begin();
   unsigned long startAttemptTime = millis();
lcd.begin(LCD_COLUMNS, LCD_ROWS);
  int status; // Declare the status for the LCD
  status = lcd.begin(LCD_COLUMNS, LCD_ROWS);
if(status) // non zero status means it was unsuccesful
{
status = -status; // convert negative status value to positive number
// begin() failed so blink error code using the onboard LED if possible
hd44780::fatalError(status); // does not return
}
  pinMode(pH_pin, INPUT);
  pinMode(DO_pin, INPUT);
  // WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < wifiTimeout){
    Serial.print("Attempting to connect");
    lcd.setCursor(0, 0);  // Line 1, Column 0
    lcd.print("WiFi Connecting");
    lcd.clear();
  }
  if (WiFi.status() == WL_CONNECTED) {

    lcd.clear();
    lcd.setCursor(1, 1);  // Line 1, Column 0
    lcd.print("WiFi Connected!");
    delay(1000);
    lcd.clear();
    Serial.println("\nConnected.");
}

}

void loop() {

  HTTPClient http;

    // The server URL with the PHP script
    String serverName = "https://example.com/data.php?";

  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);

  // pHSensor = analogRead(pH_pin);
  TDS_Sensor = analogRead(DO_pin);
  // NH3Value = analogRead(NH3_pin);

  int sensorValue = analogRead(pH_pin);
  voltage = sensorValue * (3.3 / 4095.0); // ESP32 with 3.3V reference and 12-bit resolution
  // Calculate the pH value based on the calibrated factor
  pHValue = voltage * calibration_factor;
  // float voltage=pHSensor*(3.3/4095.0);
  // pHValue=(3.3*voltage);
   
  float Voltage = TDS_Sensor*5/1024.0; //Convert analog reading to Voltage
  DOValue=(133.42*Voltage*Voltage*Voltage - 255.86*Voltage*Voltage + 857.39*Voltage)*0.5; //Convert voltage value to TDS value
  DOValue = DOValue/1000.0;
  
  data = String("&Temp=")+temperature+String("&PH=")+ph_act+String("&Tur=")+DOValue+String("&Id=");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("pH Value: ");
  Serial.println(pHValue);

  Serial.print("Turbidity Value: ");
  Serial.println(DOValue);

  // Serial.print("Ammonia Value: ");
  // Serial.println(NH3Value);

  delay(2000);

  lcd.setCursor(0, 0);  // Line 1, Column 0
  lcd.print("T = ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);  // Line 2, Column 0
  lcd.print("pH = ");
  lcd.print(pHValue);

  lcd.setCursor(0, 2);  // Line 3, Column 0
  lcd.print("Tur = ");
  lcd.print(DOValue);
  lcd.print(" ppm");
  // Combine URL and data
    String fullURL = serverName + data;

    // Start the HTTP GET request
    http.begin(fullURL);
    
    // Send the request
    int httpResponseCode = http.GET();
    
    // Check the response
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending GET: ");
      Serial.println(httpResponseCode);
    }

    // Free resources
    http.end();
delay(5000); // send data every 1 second
}
