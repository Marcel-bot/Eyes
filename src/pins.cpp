#include "includes.h"

void pins_init() {
    pinMode(TFT_CS_1, OUTPUT);
    pinMode(TFT_CS_2, OUTPUT);
    // Also initialize backlight and accelerometer
}