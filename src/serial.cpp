#include "main.h"

String buffer;
u_int8_t bufferIndex = 0;

bool rendered_once = false;

Point currentPoints[100];
int currentPointsLength;
int current_x, current_y;

Point ip(const Point a, const Point b, const float t) {
    return {
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t
    };
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

void render_current(const int x, const int y) {
    sprite.fillScreen(TFT_BLACK);
    sprite.pushToSprite(&eye, x, y);

    sprite.pushSprite(0, 0);
}

float bezier(const float start, const float control1, const float control2, const float end, const float t) {
    const float oneMinusT = t - 1;
    return oneMinusT * oneMinusT * oneMinusT * start + 3 * oneMinusT * oneMinusT * t * control1 + 3 * oneMinusT * t * t * control2 + t * t * t * end;
}

void execute_move(float params[200], const int len) {
    const unsigned long start = millis();

    for (unsigned long now = millis(); now < start + params[0] * 1000; now = millis()) {
        const float t = static_cast<float>(now - start) / (params[0] * 1000);

        render_current(bezier(current_x, params[1], params[3], params[5], t), bezier(current_y, params[2], params[4], params[6], t));
    }

    current_x = params[5];
    current_y = params[6];
}

void execute_display(float params[200], const int len) {
    const unsigned long start = millis();

    Point newPoints[100];
    int newPointsIndex = 0;

    for (int i = 8; i < len; i += 2) {
        newPoints[newPointsIndex].x = params[i];
        newPoints[newPointsIndex++].y = params[i + 1];
    }

    // TODO: Complete with missing points and rotate + shift

    for (unsigned long now = millis(); now < start + params[0] * 1000; now = millis()) {
        const float t = static_cast<float>(now - start) / (params[0] * 1000);

        float firstTerm[101], secondTerm[101], thirdTerm[101], forthTerm[101];

        float y = 0;

        for (int i = 0; i <= 100; ++i) {
            firstTerm[i] = (1 - y) * (1 - y) * (1 - y);
            secondTerm[i] = 3 * (1 - y) * (1 - y) * y;
            thirdTerm[i] = 3 * (1 - y) * y * y;
            forthTerm[i] = y * y * y;

            y += 0.01;
        }

        for (int i = 0; i < newPointsIndex; i += 3) {                
            drawCubicBezier(
                ip(newPoints[i], currentPoints[i], t),
                ip(newPoints[i + 1], currentPoints[i + 1], t),
                ip(newPoints[i + 2], currentPoints[i + 2], t),
                ip(newPoints[i + 3], currentPoints[i + 3], t),
                MARCEL_BLUE,
                firstTerm,
                secondTerm,
                thirdTerm,
                forthTerm
            );
        }

        render_current(bezier(current_x, params[1], params[3], params[5], t), bezier(current_y, params[2], params[4], params[6], t));
    }

    for (int i = 0; i < newPointsIndex; ++i) {
        currentPoints[i] = newPoints[i];
    }

    currentPointsLength = newPointsIndex;
}

void process_command(String command) {
    command.trim();
    const char cmd = command.charAt(0);

    const String paramString = command.substring(2);
    float params[200];
    int len = 0;


    int startIndex = 0;
    int spaceIndex = paramString.indexOf(' ');

    while (spaceIndex >= 0) {
        params[len++] = paramString.substring(startIndex, spaceIndex).toFloat();
        startIndex = spaceIndex + 1;
        spaceIndex = paramString.indexOf(' ', startIndex);
    }

    if (startIndex < paramString.length()) {
        params[len++] = paramString.substring(startIndex).toFloat();
    }

    switch (cmd) {
    case 'M':
        execute_move(params, len);
        break;
    
    case 'D':
        execute_display(params, len);
        break;
    }
}

void setup_uart() {
    Serial.begin(BAUDRATE);
}

void handle_uart() {
    while (Serial.available()) {
        const u_int8_t c = Serial.read();

        if (c == '\n') {
            process_command(buffer);
            buffer = "";
        } else {
            buffer += c;
        }
    }
}
