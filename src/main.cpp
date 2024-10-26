#include "main.h"

void setup() {
    setup_uart();
    setup_render();
}

void loop() {
    handle_uart();
    render();
}
