const int PIR_SENSOR_OUTPUT_PIN = 14;  // Use GPIO 14 (safe for ESP32-CAM)
const int OUTPUT_PIN = 12;             // GPIO to trigger when motion is detected

int warm_up = 0;

void setup() {
  pinMode(PIR_SENSOR_OUTPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);         // Set output pin as output
  digitalWrite(OUTPUT_PIN, LOW);       // Start with it off

  Serial.begin(115200);
  Serial.println("Waiting for PIR sensor to warm up...");
  delay(30000);  // 30-second warm-up
  Serial.println("Ready!");
}

void loop() {
  int sensor_output = digitalRead(PIR_SENSOR_OUTPUT_PIN);

  if (sensor_output == LOW) {
    if (warm_up == 1) {
      Serial.println("Warming Up...");
      warm_up = 0;
      delay(2000);
    }
    Serial.println("No object in sight");
    digitalWrite(OUTPUT_PIN, LOW);    // Turn off output pin
    delay(1000);
  } else {
    Serial.println("Object detected!");
    digitalWrite(OUTPUT_PIN, HIGH);   // Turn on output pin
    warm_up = 1;
    delay(1000);
  }
}
