#include <SPI.h>
#include <EEPROM.h>
#include <Arduboy.h>

#define WIDTH (128)
#define HEIGHT (64)

Arduboy arduboy;

uint32_t count = 0;

// What point on the screen we're using as the center of the circle.
int refX = WIDTH / 2;
int refY = HEIGHT / 2;

void setup() {
  arduboy.start();
  arduboy.setFrameRate(60);
}

void loop() {
  if (!(arduboy.nextFrame()))
    return;

  // This part makes the pattern on the screen animate.
  // The count actually increases exponentially, due to the 1.01 * count part
  // This is why the pattern starts slow and then speeds up, before going crazy.
  count = ceil(1.01 * count) + 1;
  
  // Here we actually draw the pattern on the screen.
  draw(count);
}

void draw(uint32_t count) {
    // Clear all the pixels, which essentially sets everything to black.
    arduboy.clearDisplay();
    
    // Loop through each pixel, and do a calculation. Depending on the result we'll set some pixels to white
    for (int x = 0; x < WIDTH; x ++) {
        // Calculate how far away this pixel is from the center of the screen in the x dimension
        int dx = x - refX; 
        for (int y = 0; y < HEIGHT; y ++) {
            // Calculate how far away this pixel is from the center of the screen in the y dimension
            int dy = y - refY;
            
            // This is the somewhat aribrary calculation that determines the pattern.
            
            // The (dx * dx + dy * dy) part is something that increases based on the
            // distance from the center of the screen. 
            // The (dx * dy) part skews the circle so it becomes more of an eclipse.
            // Finally, the count part is the part that makes it animated
            int dr = (dx * dx + dy * dy) + (dx * dy) + count;
            // The dr value is quite big. We only need a few values, so we shift it down
            // to make it smaller, and then take the last two bits (that's what the & 0x3 part does)
            int val = (dr >> 9) & 0x3;
            
            // val is either 0, 1, 2 or 3
            switch (val) {
                case 2:
                    // if 2, this pixel is part of the white section
                    arduboy.drawPixel(x, y, WHITE);
                    break;
                case 1:
                case 3:
                    // if it's 1 or 3, it's part of the 'grey' part.
                    // we check whether the pixel should be dithered by looking
                    // at the x and y coordinate. There are three options here,
                    // try experimenting with each to see what they do.
                
                    //if ((y ^ x) & 1) {
                    //if (x & 1) {
                    if (y & 1) {
                        arduboy.drawPixel(x, y, WHITE);
                    }
                    break;
            }
        }
    }
    
    arduboy.display();
}