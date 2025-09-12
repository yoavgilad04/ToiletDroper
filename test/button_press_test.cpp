#include <Arduino.h>
#include <WiFi.h>

// Pin definitions (adjust according to your ESP32 board)
#define LED_PIN 2
#define BUTTON_PIN 0

// WiFi credentials (uncomment and fill when ready to use WiFi)
// const char* ssid = "your-wifi-ssid";
// const char* password = "your-wifi-password";

// Variables
bool ledState = false;
unsigned long previousMillis = 0;
const long interval = 1000; // Blink interval in milliseconds

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println();
  Serial.println("=== ToiletDroper ESP32 Project ===");
  Serial.println("Initializing...");

  // Initialize pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initial LED state
  digitalWrite(LED_PIN, LOW);

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
  Serial.println("Framework: Arduino");
  Serial.println("Free heap: " + String(ESP.getFreeHeap()) + " bytes");
}

void loop() {
  unsigned long currentMillis = millis();

  // Blink LED every second
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    Serial.println("LED state: " + String(ledState ? "ON" : "OFF"));
  }

  // Check button press (optional - for testing)
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); // Simple debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      Serial.println("Button pressed!");
      // Add button functionality here
      while (digitalRead(BUTTON_PIN) == LOW) {
        delay(10); // Wait for button release
      }
    }
  }

  // Small delay to prevent excessive CPU usage
  delay(10);
}
