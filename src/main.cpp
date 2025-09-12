#include <Arduino.h>
#include <WiFi.h>
#include "MC38.h"

// Pin definitions
#define MC38_PIN 4        // MC-38 magnetic sensor on GPIO 4 (D4)
#define BUTTON_PIN 0

// WiFi credentials (uncomment and fill when ready to use WiFi)
// const char* ssid = "your-wifi-ssid";
// const char* password = "your-wifi-password";

// Create MC38 sensor object
MC38 sensor(MC38_PIN);

// Variables for detection tracking
bool previous_magnet_state = true;  // true = apart, false = together

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println();
  Serial.println("=== ToiletDroper ESP32 Project ===");
  Serial.println("MC-38 Magnetic Sensor Test (Using MC38 Class)");
  Serial.println("Initializing...");

  // Initialize button pin (MC-38 is handled by the class constructor)
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Get initial state from sensor
  previous_magnet_state = sensor.AreMagnetApart();

  // WiFi setup (commented out for now)
  /*
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  */

  Serial.println("Setup complete!");
  Serial.println("Board: ESP32");
  Serial.println("MC-38 sensor ready on GPIO " + String(sensor.getGPIOPin()) + " (D4)");
  previous_magnet_state = sensor.AreMagnetApart();
  Serial.println("Initial MC-38 state: " + String(previous_magnet_state ? "APART" : "TOGETHER"));
  Serial.println("Free heap: " + String(ESP.getFreeHeap()) + " bytes");
  Serial.println("--- Waiting for MC-38 detection ---");
}

void loop() {
  // Get current magnet state using the MC38 class
  bool current_magnet_state = sensor.AreMagnetApart();
  
  // DEBUG: Print raw pin reading every 2 seconds to see what's happening
  static unsigned long lastDebugPrint = 0;
  if (millis() - lastDebugPrint > 2000) {
    Serial.println("DEBUG: Raw pin reading = " + String(sensor.getRawReading()) + " (HIGH=1, LOW=0)");
    Serial.println("       AreMagnetApart() = " + String(current_magnet_state ? "true" : "false"));
    lastDebugPrint = millis();
  }
  
  // Check if magnet state has changed
  if (current_magnet_state != previous_magnet_state) {
    Serial.println("=== STATE CHANGE CONFIRMED ===");
    
    if (current_magnet_state) {
      // Magnets are now apart (back to default)
      Serial.println("Magnets separated (back to default)");
    } else {
      // Magnets came together - DETECTION EVENT!
      Serial.println("I've detected you mc38! (Magnets TOGETHER)");
    }
    
    // Print timestamp for reference
    Serial.println("   Time: " + String(millis()) + " ms");
    Serial.println();
    
    // Update previous state
    previous_magnet_state = current_magnet_state;
  }

  // Check button press (kept for additional testing if needed)
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); // Simple debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      Serial.println("🔘 Button pressed! (GPIO 0)");
      while (digitalRead(BUTTON_PIN) == LOW) {
        delay(10); // Wait for button release
      }
    }
  }

  // Small delay to prevent excessive CPU usage
  delay(10);
}
