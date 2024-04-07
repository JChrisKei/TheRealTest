#include "esp_camera.h"
#include <Arduino.h>

#include <WiFi.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"


// Pin Configuration
const int piezoPin = 12; // Connect the piezo buzzer to pin 12

// Camera Configuration
const int imgWidth = 320;
const int imgHeight = 240;

// Motion Detection Configuration
const int motionThreshold = 5000; // Adjust this threshold based on your requirement


#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"




void setup() {
  Serial.begin(115200);
  pinMode(piezoPin, OUTPUT);

  // Camera setup
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
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;

  // Initialize the camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

void loop() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Analyze the image for motion
  int motionScore = analyzeForMotion(fb->buf, fb->len);

  // If motion is detected, trigger the alarm
  if (motionScore > motionThreshold) {
    Serial.println("Motion Detected");
    triggerAlarm();
  }

  esp_camera_fb_return(fb);
}

int analyzeForMotion(uint8_t *imgBuf, size_t len) {
  // Perform motion detection algorithm here
  // Compare consecutive frames, calculate motion score
}

void triggerAlarm() {
  tone(piezoPin, 1000, 2000); // Sound the piezo buzzer for 2 seconds
  delay(5000); // Wait for 5 seconds to prevent repeated alarms
}
