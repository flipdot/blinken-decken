#include <Arduino.h>
#include <FastLED.h>
#define NUM_LEDS 49
#define DATA_PIN 2
#define BAUDRATE 115200

// Wolfram Rule
// http://mathworld.wolfram.com/images/eps-gif/ElementaryCARule030_1000.gif
//uint8_t RULE = 0;
uint8_t RULE = 30;
//uint8_t RULE = 16;

CHSV last_hsvs[NUM_LEDS];
CHSV hsvs[NUM_LEDS];

CRGB last_rgbs[NUM_LEDS];
CRGB rgbs[NUM_LEDS];

CRGB leds[NUM_LEDS];

uint8_t direction = 1;

// JOnas Cyan
#define COLOR1 CRGB(0, 121, 64)
#define COLOR2 CRGB(9, 223, 118)

#define COLOR1_HIGHLIGHT CRGB(0, 0, 255)
#define COLOR2_HIGHLIGHT CRGB(255, 0, 0)

uint8_t constColor[3] = {0, 121, 64};

// #define COLOR1 CRGB(255, 80, 80);
// #define COLOR2 CRGB(80, 80, 255);

void random_initialize_rgbs() {
    for (int i=0; i<NUM_LEDS; i++) {
        if (random(100) < 10) {
            rgbs[i] = COLOR1;
        } else {
            rgbs[i] = COLOR2;
        }
    }
}

void random_flip_rgb() {
    for (int i=0; i<random(4); i++) {
        int j = random(NUM_LEDS);
        rgbs[j] = random(10) > 5 ? COLOR1_HIGHLIGHT : COLOR2_HIGHLIGHT;
    }
    return;
}

void fade_to_constant() {
    for (int i=0; i<NUM_LEDS; i++) {
        for (int j=0; j<sizeof(constColor); j++) {
            int x = rgbs[i][j];
            if (x < constColor[j]) {
                x += random(3);
            } else if (x > constColor[j]) {
                x -= random(3);
            }
            rgbs[i][j] = x;
        }
    }
}

void show() {
    memcpy(last_rgbs, rgbs, sizeof(rgbs));
    for (int i=0; i < NUM_LEDS; i++) {
        // leds[i] = CRGB(hsvs[i]);
        leds[i] = rgbs[i];
    }
    FastLED.show();
}

void wolfram_iteration() {
    for (int i=0; i<NUM_LEDS; i++) {
        int n = 0;
        int next = i + 1;
        int last = i - 1;
        if (next == NUM_LEDS) {
            next = 0;
        }
        if (last == -1) {
            last = NUM_LEDS - 1;
        }

        n |= last_rgbs[next][0] != 0 ? 1 : 0;
        n |= last_rgbs[i][0] != 0 ? 0b10 : 0;
        n |= last_rgbs[last][0] != 0 ? 0b100 : 0;

        if ((RULE >> n) & 1 == 1) {
            rgbs[i] = COLOR1;
        } else {
            rgbs[i] = COLOR2;
        }
    }
}

void setup() {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    Serial.begin(BAUDRATE);
    random_initialize_rgbs();
    delay(10);
}

void loop() {
    static uint8_t k = 0;
    if (k > 50) {
        random_flip_rgb();
        //random_initialize_rgbs();
        //RULE = random(256);
        k = random(10);
    }
    // static uint8_t l = 0;
    // for (int i=0; i < NUM_LEDS; i++) {
    //     int distance = i - k;
    //     distance = abs(distance);
    //     Serial.println(distance);
    //     float brightness = 255 - distance * 4;
    //     if (brightness < 0) {
    //         brightness = 0;
    //     }
    //     Serial.println(brightness);
    //     hsvs[i] = CHSV((50 + ((i+l)*2) % 80), 255, (int) brightness);
    // }
    // k += direction;
    // if (k >= (NUM_LEDS - 1) || k <= 0) {
    //     direction *= -1;
    // }
    // l++;
    show();
    //wolfram_iteration();
    fade_to_constant();
    k++;
    delay(100);
}
