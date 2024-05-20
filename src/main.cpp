#include "includes.h"

TFT_eSPI tft;

uint16_t rgb_to_16bit_color(uint8_t red, uint8_t green, uint8_t blue)
{
  return ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
}

uint16_t z = 0;

uint16_t blue = rgb_to_16bit_color(48, 131, 220);
uint16_t red = rgb_to_16bit_color(255, 0, 0);

void update_ui(int eye, int color)
{
  uint32_t p = 0;

  uint16_t buffer[1024]{};

  digitalWrite(eye, LOW);

  tft.fillScreen(TFT_BLACK);

  tft.fillEllipse(120, 120, 80, 60, color);

  tft.endWrite();
  digitalWrite(eye, HIGH);
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

void serial_init()
{
  Serial.begin(BAUDRATE);
}

void show_error(int code)
{
  Serial.print("Error ");
  Serial.println(code);
}

void pins_init()
{
  pinMode(TFT_CS_1, OUTPUT);
  pinMode(TFT_CS_2, OUTPUT);
  // Also initialize backlight and accelerometer
}

u_int64_t start;

void setup()
{
  Serial.begin(115200);

  serial_init();
  pins_init();
  draw_init();

  start = millis();

  delay(1000);

  update_ui(21, blue);
  update_ui(22, blue);
}

float x = 0;

String buffer = "";

int timer = 0;

void loop()
{
  if (Serial.available())
  {
    char c = Serial.read();

    if (c == '\n')
    {
      if (buffer[0] == 'c')
      {
        uint16_t color = rgb_to_16bit_color(buffer[1], buffer[2], buffer[3]);

        update_ui(21, color);
        update_ui(22, color);
      }
      else if (buffer[0] == 'a')
      {
        digitalWrite(21, LOW);
        digitalWrite(22, LOW);

        for (int i = 5; i < 200; i += 30)
        {
          tft.fillRect(0, 0, 240, i, TFT_BLACK);

          delay(1);
        }

        digitalWrite(21, HIGH);
        digitalWrite(22, HIGH);

        update_ui(21, blue);
        update_ui(22, blue);
      }

      buffer = "";
    }
    else
    {
      buffer += c;
    }
  }

  if (timer == 40)
  {
    timer = 0;

    digitalWrite(21, LOW);
    digitalWrite(22, LOW);

    for (int i = 5; i < 200; i += 30)
    {
      tft.fillRect(0, 0, 240, i, TFT_BLACK);

      delay(1);
    }

    digitalWrite(21, HIGH);
    digitalWrite(22, HIGH);

    update_ui(21, blue);
    update_ui(22, blue);
  }

  timer++;

  delay(100);
}