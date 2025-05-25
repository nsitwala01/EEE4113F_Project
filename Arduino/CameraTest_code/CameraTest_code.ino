#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include "esp_camera.h"

// Replace with your Wi-Fi credentials
const char* ssid = "ANJ Studios";
const char* password = "";
//const char* ssid = "nubia Z50";
//const char* password = "0123456789";
// Replace with the IP address of your Flask server
const char* serverUrl = "http://192.168.0.5:5000/classify";
//const char* serverUrl = "http://196.47.206.154:5000/classify";
// Camera module pins for AI Thinker ESP32-CAM
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

// 4 for flash led or 33 for normal led
#define LED_GPIO_NUM   4

void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  // Wait for Wi-Fi to connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP Address: " + WiFi.localIP().toString());

  // Camera initialization
  Serial.println("Initializing camera...");
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Use a smaller frame size to avoid memory issues
  config.frame_size = FRAMESIZE_SVGA; // Use QQVGA for smaller images
  config.jpeg_quality = 10;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x", err);
    return;
  }

  delay(3000);  // Wait for camera to be fully ready
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    camera_fb_t* fb = esp_camera_fb_get();  // Capture frame from camera
    if (!fb) {
      Serial.println("Camera capture failed");
      return;
    }

    // Send image via HTTP POST to Flask server
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/octet-stream");

    // Send image data
    int httpResponseCode = http.POST(fb->buf, fb->len);
    if (httpResponseCode > 0) {
      String response = http.getString();  // Get prediction result
      Serial.println("Prediction: " + response);
    } else {
      Serial.printf("Error sending POST: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
    esp_camera_fb_return(fb);  // Return the frame buffer to free memory
  } else {
    Serial.println("WiFi not connected.");
  }

  delay(1000); // Wait 1 seconds before capturing another image
}
