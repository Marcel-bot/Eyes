#include "includes.h"

u_int64_t start;

void setup() {
  serial_init();
  pins_init();
  draw_init();

  start = millis();
}

float x = 0;

void loop() {
  update_ui();

  Serial.println(++x / (millis() - start) * 1000);
}