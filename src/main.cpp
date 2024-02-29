#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "sdkconfig.h"

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// put function declarations here:

void blink_task(void *pvParameter)
{
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, 48, NEO_GRB + NEO_KHZ800);
  strip.begin();
  
  strip.show();

  while (1)
  {
    Serial.printf("Size of strip - %d \n", sizeof(strip));
    strip.setPixelColor(0, strip.Color(255, 0, 255));
    strip.show();
    delay(1000);
    strip.setPixelColor(0, strip.Color(0, 0, 0));
    strip.show();
    delay(500);
  }
}

void setup()
{
  Serial.begin(115200);
  xTaskCreate(&blink_task, "blink_task", 2000, NULL, 5, NULL);
}

void loop()
{
  Serial.println("Hi");
  delay(500);
}