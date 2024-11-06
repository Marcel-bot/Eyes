#include "main.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

uint16_t rgb_to_16bit_color(const uint8_t red, const uint8_t green, const uint8_t blue) {
    return ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
}

void setup_render() {
    pinMode(TFT_CS_LEFT, OUTPUT);
    pinMode(TFT_CS_RIGHT, OUTPUT);

    tft.init();

    digitalWrite(TFT_CS_LEFT, LOW);
    tft.setRotation(TFT_ROTATION_LEFT);
    tft.fillScreen(TFT_BLACK);
    digitalWrite(TFT_CS_LEFT, HIGH);

    digitalWrite(TFT_CS_RIGHT, LOW);
    tft.setRotation(TFT_ROTATION_RIGHT);
    tft.fillScreen(TFT_BLACK);
    digitalWrite(TFT_CS_RIGHT, HIGH);

    sprite.createSprite(239, 239);

    delay(1000);

    digitalWrite(TFT_CS_LEFT, LOW);
}
