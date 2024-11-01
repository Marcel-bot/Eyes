#ifndef RENDER
#define RENDER

#define TFT_ROTATION_LEFT 0
#define TFT_ROTATION_RIGHT 0

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240

#define MARCEL_BLUE 13339

void setup_render();

extern auto tft = TFT_eSPI();
extern auto sprite = TFT_eSprite(&tft);
extern auto eye = TFT_eSprite(&tft);

struct Point {
    float x, y;
};

#endif