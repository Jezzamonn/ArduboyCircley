#include <SPI.h>
#include <EEPROM.h>
#include <Arduboy.h>

#define WIDTH (128)
#define HEIGHT (64)

Arduboy arduboy;

uint32_t count = 0;

int refX = WIDTH / 2;
int refY = HEIGHT / 2;

void setup() {
  arduboy.start();
  arduboy.setFrameRate(60);
}

void loop() {
  if (!(arduboy.nextFrame()))
    return;

  count = ceil(1.01 * count) + 1;
  draw();
}

void draw() {
    arduboy.clearDisplay();
    
    for (int x = 0; x < WIDTH; x ++) {
        int dx = x - refX; 
        for (int y = 0; y < HEIGHT; y ++) {
            int dy = y - refY;
            int dr = (dx * dx + dy * dy) + dx * dy + count;
            int val = (dr >> 9) & 0x3;
            switch (val) {
                case 2:
                    arduboy.drawPixel(x, y, WHITE);
                    break;
                case 1:
                case 3:
                    //if ((y ^ x) & 1) {
                    //if ((y ^ x) & 1) {
                    if (y & 1) {
                        arduboy.drawPixel(x, y, WHITE);
                    }
                    break;
            }
        }
    }
    
    arduboy.display();
}