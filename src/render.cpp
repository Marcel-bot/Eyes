#include "main.h"

auto tft = TFT_eSPI();
auto sprite = TFT_eSprite(&tft);

struct Point {
    float x, y;
};

struct Bezier {
    Point start, f1, f2, end;
};

Point ip(const Point a, const Point b, const float t) {
    return {
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t
    };
}

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

    sprite.createSprite(160, 160);

    delay(1000);

    digitalWrite(TFT_CS_LEFT, LOW);
}

void drawCubicBezier(const Point p0, const Point p1, const Point p2, const Point p3, const uint16_t color,
                     const float firstTerm[], const float secondTerm[], const float thirdTerm[],
                     const float forthTerm[]) {
    float prevX = p0.x;
    float prevY = p0.y;

    for (int i = 0; i <= 100; i += 2) {
        const float x = firstTerm[i] * p0.x + secondTerm[i] * p1.x + thirdTerm[i] * p2.x + forthTerm[i] * p3.x;

        const float y = firstTerm[i] * p0.y + secondTerm[i] * p1.y + thirdTerm[i] * p2.y + forthTerm[i] * p3.y;

        sprite.drawWideLine(prevX, prevY, x, y, 3, color);

        prevX = x;
        prevY = y;
    }
}

Bezier initial[] = {
    {{100, 68}, {97, 77}, {92, 84}, {85, 90}},
    {{85, 90}, {78, 96}, {70, 100}, {61, 101}},
    {{61, 101}, {52, 103}, {42, 102}, {34, 98}},
    {{34, 98}, {25, 95}, {18, 89}, {13, 82}},
    {{13, 82}, {7, 74}, {4, 66}, {3, 57}},
    {{3, 57}, {3, 47}, {4, 38}, {8, 30}},
    {{8, 30}, {12, 22}, {19, 15}, {27, 10}},
    {{27, 10}, {34, 6}, {43, 3}, {52, 3}},
    {{52, 3}, {67, 24}, {83, 45}, {100, 68}}
};

Bezier final[] = {
    {{101, 52}, {101, 52}, {101, 52}, {101, 52}},
    {{101, 52}, {101, 52}, {101, 52}, {101, 52}},
    {{101, 52}, {66, 52}, {33, 52}, {3, 52}},
    {{3, 52}, {3, 52}, {3, 52}, {3, 52}},
    {{3, 52}, {3, 52}, {3, 52}, {3, 52}},
    {{3, 52}, {3, 52}, {3, 52}, {3, 52}},
    {{3, 52}, {4, 25}, {25, 3}, {52, 3}},
    {{52, 3}, {52, 3}, {52, 3}, {52, 3}},
    {{52, 3}, {79, 3}, {100, 25}, {101, 52}}
};

void render() {
    sprite.fillScreen(TFT_BLACK);

    float firstTerm[101], secondTerm[101], thirdTerm[101], forthTerm[101];

    float t = 0;

    for (int i = 0; i <= 100; ++i) {
        firstTerm[i] = (1 - t) * (1 - t) * (1 - t);
        secondTerm[i] = 3 * (1 - t) * (1 - t) * t;
        thirdTerm[i] = 3 * (1 - t) * t * t;
        forthTerm[i] = t * t * t;

        t += 0.01;
    }

    const unsigned long time = millis();

    for (float t = 0.0; t < 1.0; t += .03) {
        sprite.fillScreen(TFT_BLACK);

        for (int i = 0; i < 9; i++) {
            drawCubicBezier(
                ip(initial[i].start, final[i].start, t),
                ip(initial[i].f1, final[i].f1, t),
                ip(initial[i].f2, final[i].f2, t),
                ip(initial[i].end, final[i].end, t),
                MARCEL_BLUE,
                firstTerm,
                secondTerm,
                thirdTerm,
                forthTerm
            );
        }

        sprite.pushSprite(70, 70);
    }

    delay(2000);

    Serial.println(millis() - time);
}
