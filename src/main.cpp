#include <Arduino.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#include "config.hpp"

using namespace std;

CRGB led_buf_co2[NUM_LEDS];
CRGB led_buf_usr[NUM_LEDS];
CRGB led_buf[NUM_LEDS];

double heartbeat = 0;

void buf_random(CRGB buf[], size_t size) {
    for (int i = 0; i < size; i++) {
        if (random(100) < 50) {
            buf[i] = C_CYAN;
        } else {
            buf[i] = C_RED;
        }
    }
}

void buf_grey(CRGB buf[], size_t size) {
    for (int i = 0; i < size; i++) {
        buf[i] = C_GREY;
    }
}

void led_stripe_update(double heartbeat, CRGB buf[], CRGB buf_co2[], CRGB buf_usr[], size_t size) {
    // Mix defines the ratio in which to mix the buf_co2 and buf_usr channels.
    // mix =   0 : Display only buf_co2
    // mix = 0.5 : Display 50% buf_co2 and 50% buf_usr
    // mix =   1 : Display only buf_usr
    double mix = (sin(heartbeat) + 1) / 2;
    Serial.println(mix);

    for (int i = 0; i < size; i++) {
        buf[i] = CRGB(
            buf_co2[i].r * mix + buf_usr[i].r * (1 - mix),
            buf_co2[i].g * mix + buf_usr[i].g * (1 - mix),
            buf_co2[i].b * mix + buf_usr[i].b * (1 - mix)
        );
    }

    FastLED.show();
}

void setup() {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(led_buf, NUM_LEDS);
    Serial.begin(BAUDRATE);

    buf_random(led_buf_co2, NUM_LEDS);
    buf_grey(led_buf_usr, NUM_LEDS);
}

void loop() {
    led_stripe_update(heartbeat, led_buf, led_buf_co2, led_buf_usr, NUM_LEDS);
    delay(FRAME_SLEEP);

    heartbeat += HEARTBEAT_INC;
}
