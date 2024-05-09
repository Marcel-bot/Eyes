#include "includes.h"

TFT_eSPI tft;

uint16_t rgb_to_16bit_color(uint8_t red, uint8_t green, uint8_t blue)
{
    return ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
}

uint16_t z = 0;

void update_ui()
{
    uint32_t p = 0;

    uint16_t buffer[1024]{};

    digitalWrite(21, LOW);

    tft.startWrite();
    tft.setAddrWindow(0, z - 10, 240, 20);

    for (uint8_t x = 0; x < 240; ++x)
    {
        for (uint8_t y = 0; y < 10; ++y)
        {
                            buffer[p++] = TFT_BLACK;
            
            if (p == 1024)
            {
                tft.pushColors(buffer, p);
                p = 0;
            }
        }
    }

    for (uint8_t x = 0; x < 240; ++x)
    {
        for (uint8_t y = 0; y < 10; ++y)
        {
            buffer[p++] = TFT_RED;

            if (p == 1024)
            {
                tft.pushColors(buffer, p);
                p = 0;
            }
        }
    }

    z += 10;

    if (p)
    {
        tft.pushColors(buffer, p);
        p = 0;
    }

    tft.endWrite();
    digitalWrite(21, HIGH);
}

void draw_init()
{
    tft.init();

    digitalWrite(21, LOW);
    tft.fillScreen(0);
    digitalWrite(21, HIGH);

    digitalWrite(22, LOW);
    tft.fillScreen(0);
    digitalWrite(22, HIGH);
}