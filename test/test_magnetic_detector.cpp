#include <Arduino.h>
#include <WiFi.h>

// Pin definitions
#define MC38_PIN 4        // MC-38 magnetic sensor on GPIO 4 (D4)
#define BUTTON_PIN 0

// WiFi credentials (uncomment and fill when ready to use WiFi)
// const char* ssid = "your-wifi-ssid";
// const char* password = "your-wifi-password";

// Variables for MC-38 detection
bool mc38_current_state = HIGH;     // Current state of MC-38
bool mc38_previous_state = HIGH;    // Previous state to detect changes
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;   // Debounce time in milliseconds

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println();
  Serial.println("=== ToiletDroper ESP32 Project ===");
  Serial.println("MC-38 Magnetic Sensor Test");
  Serial.println("Initializing...");

  // Initialize pins
  pinMode(MC38_PIN, INPUT_PULLUP);  // MC-38 sensor with internal pull-up
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Read initial state of MC-38
  mc38_current_state = digitalRead(MC38_PIN);
  mc38_previous_state = mc38_current_state;

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
  Serial.println("MC-38 sensor ready on GPIO 4 (D4)");
  Serial.println("Initial MC-38 state: " + String(mc38_current_state == LOW ? "OPEN (magnet away)": "CLOSED (magnet near)"));
  Serial.println("Free heap: " + String(ESP.getFreeHeap()) + " bytes");
  Serial.println("--- Waiting for MC-38 detection ---");
}

void loop() {
  // Read current state of MC-38 sensor
  int reading = digitalRead(MC38_PIN);
  
  // DEBUG: Print raw pin reading every 2 seconds to see what's happening
  static unsigned long lastDebugPrint = 0;
  if (millis() - lastDebugPrint > 2000) {
    Serial.println("DEBUG: Raw pin reading = " + String(reading) + " (HIGH=1, LOW=0)");
    lastDebugPrint = millis();
  }
  
  // Check if state has changed (for debouncing)
  if (reading != mc38_previous_state) {
    lastDebounceTime = millis();
    Serial.println("State change detected! New reading: " + String(reading));
  }

  // If enough time has passed since the last state change, accept the new state
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Only process if the state has actually changed
    if (reading != mc38_current_state) {
      mc38_current_state = reading;
      
      Serial.println("=== STATE CHANGE CONFIRMED ===");
      Serial.println("New state: " + String(mc38_current_state));
      
      // Print detection message based on state
      if (mc38_current_state == LOW) {
        // This should be when magnets come together (switch closes, pulls to ground)
        Serial.println("I've detected you mc38! (Magnets TOGETHER)");
      } else {
        // This should be when magnets are apart (switch open, pulled high)
        Serial.println("Magnets separated (back to default)");
      }
      
      // Print timestamp for reference
      Serial.println("   Time: " + String(millis()) + " ms");
      Serial.println();
    }
  }

  // Update previous state for next comparison
  mc38_previous_state = reading;

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
