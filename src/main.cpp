#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <PubSubClient.h>

#include "main.hpp"
#include "config.hpp"

using namespace std;

CRGB led_buf_co2[NUM_LEDS];
CRGB led_buf_usr[NUM_LEDS];
CRGB led_buf[NUM_LEDS];

const uint32_t CHIP_ID = ESP.getChipId();

WiFiClient wifi_client;
PubSubClient mqtt_client(MQTT_HOST, 1883, callback, wifi_client);
int heartbeat = 0;

int wifi_connect() {
    Serial.print("Connecting to WiFi.");
    WiFi.begin(SSID, PASSWORD);

    int fails = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        fails++;
        if (fails > 30) {
            Serial.println("\nConnection timed out.\n");
            return 0;
        }
    }
    return 1;
}

void buf_random(CRGB buf[], size_t size, CRGB c1, CRGB c2) {
    for (int i = 0; i < size; i++) {
        if (random(100) < 50) {
            buf[i] = c1;
        } else {
            buf[i] = c2;
        }
    }
}

void buf_copy(CRGB buf1[], CRGB buf2[], size_t size) {
    for (int i = 0; i < size; i++) {
        buf2[i] = buf1[i];
    }
}

void buf_progress_dot(CRGB buf[], size_t size, CRGB c1, CRGB c2, int heartbeat) {
    for (int i = 0; i < size; i++) {
        if ((heartbeat + i) % size == 0) {
            buf[i] = c1;
        } else {
            buf[i] = c2;
        }
    }
}

void buf_progress_bar(CRGB buf[], size_t size, CRGB c1, CRGB c2, double progress) {
    for (int i = 0; i < size; i++) {
        if (i < progress * size) {
            buf[i] = c1;
        } else {
            buf[i] = c2;
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
    double mix = (sin(heartbeat / 100) + 1) / 2 + 1;

    for (int i = 0; i < size; i++) {
        buf[i] = CRGB(
            buf_co2[i].r * mix + buf_usr[i].r * (1 - mix),
            buf_co2[i].g * mix + buf_usr[i].g * (1 - mix),
            buf_co2[i].b * mix + buf_usr[i].b * (1 - mix)
        );
    }

    FastLED.show();
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);

    Serial.print("Received message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }

    Serial.println();
}

void setup() {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(led_buf, NUM_LEDS);
    Serial.begin(BAUDRATE);
    /*
    delay(3000);

    // Initialize WiFi connection
    while (!wifi_connect()) {
        buf_random(led_buf_co2, NUM_LEDS, C_BLACK, C_YELLOW);
        buf_copy(led_buf_co2, led_buf_usr, NUM_LEDS);
        led_stripe_update(heartbeat, led_buf, led_buf_co2, led_buf_usr, NUM_LEDS);
        heartbeat++;
        FastLED.show();
    }

    Serial.printf("Connected to: %s with IP address %s\n",
            SSID,
            WiFi.localIP().toString().c_str());

    char client_id[9];
    snprintf(client_id, 8, "%X", CHIP_ID);

    mqtt_client.connect(client_id);
    Serial.println("Connected to MQTT broker.");

    mqtt_client.subscribe(MQTT_TOPIC_CO2);
    Serial.printf("Connected to MQTT topic '%s'\n.",
            MQTT_TOPIC_CO2);
    //mqtt_client.subscribe(MQTT_TOPIC_USERS);
    //Serial.printf("Connected to MQTT topic '%s'\n.",
    //        MQTT_TOPIC_USERS);

    Serial.println("Setup complete!\n");
    */
}

void loop() {
    // double progress = (sin((double)heartbeat / 90) + 1) / 2;
    // buf_progress_bar(led_buf_co2, NUM_LEDS, C_RED, C_CYAN, progress);
    buf_progress_dot(led_buf_co2, NUM_LEDS, C_YELLOW, C_BLACK, heartbeat * .23);
    buf_progress_dot(led_buf_usr, NUM_LEDS, C_RED, C_BLACK, - heartbeat * .4);

    led_stripe_update(heartbeat, led_buf, led_buf_co2, led_buf_usr, NUM_LEDS);

    /*
    if (heartbeat % 100 == 0) {
        mqtt_client.publish(MQTT_TOPIC_CO2, "hello");
    }
    */

    heartbeat++;
    delay(FRAME_SLEEP);
}
