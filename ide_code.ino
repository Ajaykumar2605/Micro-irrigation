#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include "dashboard.h" // dashboard HTML webpage contents with JavaScripts
#include "login.h"
#include "index.h" //index html page
#include "DHTesp.h" // DHT11 Library for ESP
#define LED 2 // Onboard LED
#define DHTpin D4 // D4 of NodeMCU is GPIO2
#define MOISTURE_PIN A0 // Soil moisture sensor connected to A0
#define RAIN_PIN D5 // Rain drop sensor connected to D5 (GPIO14)
#define PUMP_PIN D6 // Pump pin
DHTesp dht;
//valve pins
const int VALVE1_PIN = D1;  // Pin connected to Valve 1
const int VALVE2_PIN = D2;  // Pin connected to Valve 2
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
    digitalWrite(PUMP_PIN, LOW); // Turn off pump
    Serial.println("Pump is OFF");
  } 
  else if (sensor  < 600) { // Example threshold, adjust as needed
    digitalWrite(PUMP_PIN, LOW); // Turn on pump
    Serial.println("Pump is ON");
  } else {
    digitalWrite(PUMP_PIN, HIGH); // Turn on pump
    Serial.println("Pump is ON");
  }
  delay(1000); // Wait before checking again
  String pumpStatus = digitalRead(PUMP_PIN) == HIGH ? "1" : "0";
  server.handleClient(); // Handle client requests
}
