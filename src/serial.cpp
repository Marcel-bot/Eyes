#include "includes.h"

void serial_init() {
    Serial.begin(BAUDRATE);
}

void show_error(int code) {
    Serial.print("Error ");
    Serial.println(code);
}