#include "includes.h"
#include <stack>

TFT_eSPI tft;

struct Point {
  int x, y;
};

bool mat[240][240];

// Function to draw a quadratic Bezier curve
void drawQuadraticBezier(Point p0, Point p1, Point p2, uint16_t color) {
  float t;
  int prevX = p0.x;
  int prevY = p0.y;

  // Loop over t from 0 to 1 to draw the curve
  for (t = 0.0; t <= 1.0; t += 0.01) {
    float x = (1-t)*(1-t)*p0.x + 2*(1-t)*t*p1.x + t*t*p2.x;
    float y = (1-t)*(1-t)*p0.y + 2*(1-t)*t*p1.y + t*t*p2.y;
    
    tft.drawLine(prevX, prevY, (int)x, (int)y, color);

    mat[prevX][prevY] = true;
    
    prevX = (int)x;
    prevY = (int)y;

    mat[prevX][prevY] = true;
  }
}

// Function to implement a basic flood fill
void floodFill(int x, int y, uint16_t fillColor, uint16_t boundaryColor) {

  std::stack<Point> stack;

  stack.push({x, y});

  while (!stack.empty()) {
    Point p = stack.top();
    stack.pop();

    if (mat[p.x][p.y]) continue;

    mat[p.x][p.y] = true;
    tft.drawPixel(p.x, p.y, fillColor);

    stack.push({p.x + 1, p.y});
    stack.push({p.x - 1, p.y});
    stack.push({p.x, p.y + 1});
    stack.push({p.x, p.y - 1});
  }
}

uint16_t rgb_to_16bit_color(uint8_t red, uint8_t green, uint8_t blue)
{
  return ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
}

uint16_t z = 0;

uint16_t blue = rgb_to_16bit_color(48, 131, 220);
uint16_t red = rgb_to_16bit_color(255, 0, 0);

void update_ui(int eye, int color, bool t) {
  digitalWrite(eye, LOW);

   tft.fillScreen(TFT_BLACK);

  // Point p0 = {80, 80}, p1 = {132, 96}, p2 = {144, 120};
  // Point p3 = {144, 120}, p4 = {150, 132}, p5 = {144, 144};
  // Point p6 = {144, 144}, p7 = {132, 168}, p8 = {80, 144};
  // Point p9 = {80, 144}, p10 = {120, 132}, p11 = {80, 80};

  // drawQuadraticBezier(p0, p1, p2, TFT_BLUE);
  // drawQuadraticBezier(p3, p4, p5, TFT_BLUE);
  // drawQuadraticBezier(p6, p7, p8, TFT_BLUE);
  // drawQuadraticBezier(p9, p10, p11, TFT_BLUE);

  const float x = 100, y = 100, mulX = .8, mulY = .8;

  const float z = (t ? -1 : 1) * 50 * mulY;

  Point p0 = {x + 0 * mulX, y + 0 * mulY}, p1 = {x + 50 * mulX, y + 0 * mulY}, p2 = {x + 100 * mulX, y + 0 * mulY};
  Point p3 = {x + 0 * mulX, y + 0 * mulY}, p4 = {x + 0 * mulX, y + z}, p5 = {x + 50 * mulX, y + z};
  Point p6 = {x + 100 * mulX, y + 0 * mulY}, p7 = {x + 100 * mulX, y + z}, p8 = {x + 50 * mulX, y + z};

  drawQuadraticBezier(p0, p1, p2, TFT_BLUE);
  drawQuadraticBezier(p3, p4, p5, TFT_BLUE);
  drawQuadraticBezier(p6, p7, p8, TFT_BLUE);

  floodFill(int(x + 25 * mulX), int(y + (t ? -1 : 1) * 5 * mulY), TFT_RED, TFT_BLUE);

  tft.endWrite();
  digitalWrite(eye, HIGH);

  for (int i = 0; i < 240; i++) {
    for (int j = 0; j < 240; j++) {
      mat[i][j] = false;
    }
  }
}

void draw_init() {
  tft.init();

  digitalWrite(21, LOW);
  tft.fillScreen(0);
  digitalWrite(21, HIGH);

  digitalWrite(22, LOW);
  tft.fillScreen(0);
  digitalWrite(22, HIGH);
}

void serial_init() {
  Serial.begin(BAUDRATE);
}

void pins_init() {
  pinMode(TFT_CS_1, OUTPUT);
  pinMode(TFT_CS_2, OUTPUT);
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

  update_ui(21, blue, 1);
  update_ui(22, blue, 1);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();

    if (c == 'a') {
      update_ui(21, blue, 0);
      update_ui(22, blue, 0);
    } else {
      update_ui(21, blue, 1);
      update_ui(22, blue, 1);
    }
  }
}