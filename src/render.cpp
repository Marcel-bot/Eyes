#include "main.h"

auto tft = TFT_eSPI();
auto sprite = TFT_eSprite(&tft);

struct Point {
    int x, y;
};

uint16_t rgb_to_16bit_color(uint8_t red, uint8_t green, uint8_t blue) {
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

void drawCubicBezier(const Point p0, const Point p1, const Point p2, const Point p3, const uint16_t color) {
    int prevX = p0.x;
    int prevY = p0.y;

    for (float t = 0.0; t <= 1.0; t += 0.01) {
        const float x = (1 - t) * (1 - t) * (1 - t) * p0.x
                        + 3 * (1 - t) * (1 - t) * t * p1.x
                        + 3 * (1 - t) * t * t * p2.x
                        + t * t * t * p3.x;

        const float y = (1 - t) * (1 - t) * (1 - t) * p0.y
                        + 3 * (1 - t) * (1 - t) * t * p1.y
                        + 3 * (1 - t) * t * t * p2.y
                        + t * t * t * p3.y;

        sprite.drawWideLine(prevX, prevY, x, y, 3, color);

        prevX = static_cast<int>(x);
        prevY = static_cast<int>(y);
    }
}


void render() {
    // sprite.fillSmoothRoundRect(60, 60, 120, 120, 50, MARCEL_BLUE, TFT_BLACK);

    for (float t = 0.0; t < 1.0; t += .02) {
        sprite.fillScreen(TFT_BLACK);

        const u_int16_t color = rgb_to_16bit_color(48 + (255 - 40) * t, 131 * (1 - t), 220 * (1 - t));


        drawCubicBezier({2, 37}, {44, static_cast<int>(37 + (120 - 37) * t)}, {74, static_cast<int>(37 + (120 - 37) * t)},
                        {117, 37}, color);

        drawCubicBezier({117, 37}, {74, static_cast<int>(-11 + (80 + 11) * t)},
                        {44, static_cast<int>(-11 + (80 + 11) * t)}, {2, 37}, color);

        // drawCubicBezier({2, 37}, {44, 37}, {74, 37}, {117, 37}, MARCEL_BLUE);
        // drawCubicBezier({117, 37}, {74, -11}, {44, -11}, {2, 37}, MARCEL_BLUE);
        //
        // drawCubicBezier({2, 37}, {44, 85}, {74, 85}, {117, 37}, MARCEL_BLUE);
        // drawCubicBezier({117, 37}, {74,37}, {44, 37}, {2, 37}, MARCEL_BLUE);

        sprite.pushSprite(80, 80);
    }

    // digitalWrite(TFT_CS_LEFT, HIGH);
}
