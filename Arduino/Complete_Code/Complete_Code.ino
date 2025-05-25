#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include "esp_camera.h"

// === WiFi Settings ===
const char* ssid = "ANJ Studios";
const char* password = "";
//const char* ssid = "nubia Z50";
//const char* password = "0123456789";
// === Flask Server URL ===

const char* serverUrl = "http://192.168.0.5:5000/classify";

// === PIR and Output Pins ===
const int PIR_SENSOR_OUTPUT_PIN = 14;  // PIR sensor input
const int OUTPUT_PIN = 12;             // Output trigger pin

// === Camera Pins for AI Thinker ESP32-CAM ===
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM       5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Set up PIR and output pins
  pinMode(PIR_SENSOR_OUTPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, LOW);

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_SVGA; // or FRAMESIZE_QVGA for smaller images
  config.jpeg_quality = 10;
  config.fb_count = 1;

  // Camera init
  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    while (1);
  }

  Serial.println("System ready. Warming up PIR sensor...");
  delay(30000);  // PIR warm-up time
  Serial.println("Ready to detect motion.");
}

void loop() {
  int pirValue = digitalRead(PIR_SENSOR_OUTPUT_PIN);

  if (pirValue == HIGH) {
    Serial.println("Motion detected!");

    // Take a picture
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      return;
    }

    // Send photo to server
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverUrl);
      http.addHeader("Content-Type", "application/octet-stream");

      int httpResponseCode = http.POST(fb->buf, fb->len);
      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Prediction: " + response);

        // Check response
        if (response.indexOf("honeybadger") >= 0) {
          digitalWrite(OUTPUT_PIN, HIGH);
          Serial.println("ALERT: Honeybadger detected!");
        } else {
          digitalWrite(OUTPUT_PIN, LOW);
        }
      } else {
        Serial.printf("POST failed: %s\n", http.errorToString(httpResponseCode).c_str());
        digitalWrite(OUTPUT_PIN, LOW);
      }

      http.end();
    } else {
      Serial.println("WiFi not connected");
    }

    esp_camera_fb_return(fb);
    delay(1000); // Wait 1 second before next capture
  } else {
    Serial.println("No motion.");
    digitalWrite(OUTPUT_PIN, LOW);
    delay(500); // Lower delay for quicker detection
  }
}
