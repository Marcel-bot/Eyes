#include "main.h"

u_int8_t buffer[256];
u_int8_t bufferIndex = 0;

void process_command(const u_int8_t &c) {

}

void setup_uart() {
    Serial.begin(BAUDRATE);
}

void handle_uart() {
    while (Serial.available()) {
        const u_int8_t c = Serial.read();

        if (c == '\n') {
            bufferIndex = 0;

            process_command(buffer[0]);
        } else {
            buffer[bufferIndex++] = c;
        }
    }
}
