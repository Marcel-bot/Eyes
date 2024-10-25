#include "main.h"

TFT_eSPI tft;

void setup_render() {
    pinMode(TFT_CS_LEFT, OUTPUT);
    pinMode(TFT_CS_RIGHT, OUTPUT);

    tft.init();

    digitalWrite(TFT_CS_LEFT, LOW);
    tft.fillScreen(TFT_ROTATION_LEFT);
    digitalWrite(TFT_CS_LEFT, HIGH);

    digitalWrite(TFT_CS_RIGHT, LOW);
    tft.fillScreen(TFT_ROTATION_RIGHT);
    digitalWrite(TFT_CS_RIGHT, HIGH);
}
