#include <LiquidCrystal_PCF8574.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include "dashboard.h" // dashboard HTML webpage contents with JavaScripts
#include "login.h"
#include "index.h" //index html page
#include "DHTesp.h" // DHT11 Library for ESP
LiquidCrystal_PCF8574 lcd(0x3F);  // I2C address of the display
#define LED 2 // Onboard LED
#define DHTpin D4 // D4 of NodeMCU is GPIO2
#define MOISTURE_PIN A0 // Soil moisture sensor connected to A0
#define RAIN_PIN D5 // Rain drop sensor connected to D5 (GPIO14)
#define PUMP_PIN D6 // Pump pin
DHTesp dht;

//valve pins
const int VALVE1_PIN = D7;  // Pin connected to Valve 1
const int VALVE2_PIN = D8;  // Pin connected to Valve 2
const int VALVE3_PIN = D3;  // Pin connected to Valve 3

// WiFi credentials
const char* ssid = "MURUGAVEL";
const char* password = "19741979";
ESP8266WebServer server(80); // Server on port 80
float humidity = 0, temperature = 0;

// Handle root URL (webpage)
void handleRoot() {
  String s = dashboard_html; // Load HTML content
  server.send(200, "text/html", s); // Send web page
}
// Handle sensor data requests
void handleADC() {
  int rain = digitalRead(RAIN_PIN); // Read digital state of rain sensor
  int soilMoisture = analogRead(MOISTURE_PIN); // Soil moisture sensor reading
  // Update DHT sensor values
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
// Get pump status
  String pumpStatus = digitalRead(PUMP_PIN) == HIGH ? "0" : "1";
  // Create JSON response with sensor data
  String data = "{";
  data += "\"Rain\":\"" + String(rain) + "\",";
  data += "\"SoilMoisture\":\"" + String(soilMoisture) + "\",";
  data += "\"Temperature\":\"" + String(temperature) + "\",";
  data += "\"Humidity\":\"" + String(humidity) + "\",";
  data += "\"PumpStatus\":\"" + String(digitalRead(PUMP_PIN)) + "\"}";
  digitalWrite(LED, !digitalRead(LED)); // Toggle LED on data request
  server.send(200, "application/json", data); // Send JSON response
  // Debugging
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Rain: ");
  Serial.println(rain);
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoisture);
}
void setup() {
  Serial.begin(115200);
  Serial.println();
  // Initialize DHT sensor
  dht.setup(DHTpin, DHTesp::DHT11);
  pinMode(LED, OUTPUT);
  pinMode(RAIN_PIN, INPUT); // Rain sensor pin as input
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  // Initialize valve pins
  pinMode(VALVE1_PIN, OUTPUT);
  pinMode(VALVE2_PIN, OUTPUT);
  pinMode(VALVE3_PIN, OUTPUT);
  // Default state: Close both valves
  digitalWrite(VALVE1_PIN, LOW);
  digitalWrite(VALVE2_PIN, LOW);
  digitalWrite(VALVE3_PIN, LOW);

  // Initialize the LCD
  lcd.begin(20, 4);  // 20 columns, 4 rows
  lcd.setBacklight(1);  // Turn on the backlight

  // Show welcome message
  String text = "Welcome FarmXUi";
  int len = text.length();
  int position = (20 - len) / 2;

  lcd.setCursor(position, 1); // Center text on Row 2
  lcd.print(text);
  
  // Initial loading dots
  lcd.setCursor(0, 2);
  lcd.print("     ...");

  // Start Wi-Fi connection
  WiFi.begin(ssid, password);
  delay(100);  // Give some time for Wi-Fi to initialize

  int attempt = 0;  
  int loadingStep = 0;  // Initialize loading step

  while (WiFi.status() != WL_CONNECTED && attempt < 30) {  // 30 attempts (~15s timeout)
    delay(500);
    Serial.print(".");

    // Update LCD loading animation
    lcd.setCursor(0, 2);
    switch (loadingStep) {
        case 0: lcd.print("     ...."); break;
        case 1: lcd.print("     ......"); break;
        case 2: lcd.print("     ......."); break;
        case 3: lcd.print("     ........"); break;
    }
    loadingStep = (loadingStep + 1) % 4;  // Cycle between 0-3
    attempt++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Clear LCD after connection
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi:");
    lcd.print(ssid);

    lcd.setCursor(0, 1);
    lcd.print("IP  :");
    lcd.print(WiFi.localIP().toString());

    // Display Web URL
    String ip = WiFi.localIP().toString();
    String url = "URL :http://" + ip + "/dashboard";

    lcd.setCursor(0, 2);
    lcd.print(url.substring(0, (url.length() < 20 ? url.length() : 20)));  // First 20 characters

    lcd.setCursor(0, 3);
    if (url.length() > 20) {
        lcd.print(url.substring(20));  // Remaining characters
    }
  } else {
    Serial.println("\nWiFi Connection Failed!");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("   WiFi Failed!");
  }
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // WiFi connection successful
Serial.println();
Serial.print("Connected to ");
Serial.println(ssid);
Serial.print("IP Address: ");
Serial.println(WiFi.localIP());
Serial.print("Website for dashboard: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/dashboard");
Serial.print("Website for Management page: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/");
//Serial.println("/index");
// Serve the login page and replace %IP_ADDRESS% with the actual IP
  server.on("/", []() {
    String html = login_code;
    html.replace("%IP_ADDRESS%", WiFi.localIP().toString());
    server.send(200, "text/html", html);
  });
// Serve the HTML page on root URL
  server.on("/index", HTTP_GET, []() {
    server.send(200, "text/html", index_html);  // Serve the index HTML from index.h
  });
  // Setup server routes
  server.on("/dashboard", handleRoot); // Handle root URL
  server.on("/readADC", handleADC); // Handle sensor data requests
  server.begin(); // Start HTTP server
  Serial.println("HTTP server started");
    // Handle HTTP requests to control the valves
  server.on("/control-valve/1/on", HTTP_GET, []() {
    digitalWrite(VALVE1_PIN, LOW);  // Open Valve 1
    server.send(200, "application/json", "{\"status\":\"Valve 1 ON\"}");
  });
  server.on("/control-valve/1/off", HTTP_GET, []() {
    digitalWrite(VALVE1_PIN, HIGH);  // Close Valve 1
    server.send(200, "application/json", "{\"status\":\"Valve 1 OFF\"}");
  });
  server.on("/control-valve/2/on", HTTP_GET, []() {
    digitalWrite(VALVE2_PIN, LOW);  // Open Valve 2
    server.send(200, "application/json", "{\"status\":\"Valve 2 ON\"}");
  });
  server.on("/control-valve/2/off", HTTP_GET, []() {
    digitalWrite(VALVE2_PIN, HIGH);  // Close Valve 2
    server.send(200, "application/json", "{\"status\":\"Valve 2 OFF\"}");
  });
  server.on("/control-valve/3/on", HTTP_GET, []() {
    digitalWrite(VALVE3_PIN, LOW);  // Open Valve 3
    server.send(200, "application/json", "{\"status\":\"Valve 3 ON\"}");
  });
  server.on("/control-valve/3/off", HTTP_GET, []() {
    digitalWrite(VALVE3_PIN, HIGH);  // Close Valve 3
    server.send(200, "application/json", "{\"status\":\"Valve 3 OFF\"}");
  });
}
void loop() {
  int sensor = analogRead(MOISTURE_PIN);
  int rainsensor = digitalRead(RAIN_PIN);
  int soilMoisturePercentage = map(sensor, 0, 1023, 100, 0);
 // Pump control based on soil moisture threshold
 if (rainsensor != 1 ) { // Example threshold, adjust as needed
    digitalWrite(PUMP_PIN, LOW); // Turn on pump
    //Serial.println("Pump is OFF");
  } 
  else if (sensor  < 600) { // Example threshold, adjust as needed
    digitalWrite(PUMP_PIN, LOW); // Turn off pump
    //Serial.println("Pump is OFF");
  } else {
    digitalWrite(PUMP_PIN, HIGH); // Turn on pump
    //Serial.println("Pump is ON");
  }
  delay(1000); // Wait before checking again
  String pumpStatus = digitalRead(PUMP_PIN) == HIGH ? "1" : "0";
  server.handleClient(); // Handle client requests
}
