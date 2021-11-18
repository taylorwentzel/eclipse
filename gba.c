#include "gba.h"
#include "sprites.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;
u32 vBlankCounter = 0;

volatile OamEntry shadow[128];
volatile OamEntry *ship;
volatile OamEntry *selector;
volatile OamEntry *enemy0;
volatile OamEntry *enemy1;
volatile OamEntry *enemy2;
volatile OamEntry *enemy3;
volatile OamEntry *enemy4;
volatile OamEntry *pause;


void waitForVBlank(void) {
    // Write a while loop that loops until we're NOT in vBlank anymore:
    // (This prevents counting one VBlank more than once if your app is too fast)
    while(*SCANLINECOUNTER > 160);
    while(*SCANLINECOUNTER < 160);
    vBlankCounter++;

}

static int __qran_seed= 42;
static int qran(void) {
    __qran_seed= 1664525*__qran_seed+1013904223;
    return (__qran_seed>>16) & 0x7FFF;
}

int randint(int min, int max) {
    return (qran()*(max-min)>>15)+min;
}

void setPixel(int x, int y, volatile u16 color) {
    int location = OFFSET(y, x, 240);
    DMA[3].cnt = 0;
    DMA[3].src = &color;
    DMA[3].dst = &(videoBuffer[location]);
    DMA[3].cnt = 1 | DMA_ON | DMA_SOURCE_FIXED;
}


void drawFullScreenImageDMA(const u16 *image) {
    DMA[3].cnt = 0;
    DMA[3].src = image;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = 38400 | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT | DMA_ON;
}

void drawDeathScreen(const u16 *image) {
    int x = 88;
    int location;
    for (int y = 57; y < 71; y++) {
        location = OFFSET(y, x, 240);
        DMA[3].cnt = 0;
        DMA[3].dst = &(videoBuffer[location]);
        DMA[3].src = &(image[location]);
        DMA[3].cnt = 80 | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT | DMA_ON;
    }

    for (int y = 116; y < 134; y++) {
        location = OFFSET(y, x, 240);
        DMA[3].cnt = 0;
        DMA[3].dst = &(videoBuffer[location]);
        DMA[3].src = &(image[location]);
        DMA[3].cnt = 80 | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT | DMA_ON;
    }
}

void drawChar(int col, int row, char ch, volatile u16 color) {
    for(int r = 0; r<8; r++) {
        for(int c=0; c<6; c++) {
            if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
                setPixel(col+c, row+r, color);
            }
        }
    }
}

void drawString(int col, int row, char *str, volatile u16 color) {
    while(*str) {
        drawChar(col, row, *str++, color);
        col += 6;
    }
}

void undrawScore(int x, int y, volatile u16 color) {
    DMA[3].cnt = 0;
    x = x + 36;
    int stop = y + 7;
    int location = OFFSET(y, x, 240);

    while (y <= stop) {
        location = OFFSET(y, x, 240);
        DMA[3].cnt = 0;
        DMA[3].src = &color;
        DMA[3].dst = &(videoBuffer[location]);
        DMA[3].cnt = 30 | DMA_DESTINATION_INCREMENT | DMA_ON | DMA_SOURCE_FIXED;
        y++;
    }

}

void undrawLives(volatile u16 color) {
    int x = 230;
    int location;

    for (int y = 150; y < 160; y++) {
        DMA[3].cnt = 0;
        location = OFFSET(y, x, 240);
        DMA[3].dst = &(videoBuffer[location]);
        DMA[3].src = &color;
        DMA[3].cnt = 9 | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED | DMA_ON;
    }
}

void drawCenteredString(int x, int y, int width, int height, char *str, u16 color) {
    u32 len = 0;
    char *strCpy = str;
    while (*strCpy) {
        len++;
        strCpy++;
    }

    u32 strWidth = 6 * len;
    u32 strHeight = 8;

    int col = x + ((width - strWidth) >> 1);
    int row = y + ((height - strHeight) >> 1);
    drawString(col, row, str, color);
}

void initializeSprites(void) {
    DMA[3].cnt = 0;
    DMA[3].src = sprites_palette;
    DMA[3].dst = SPRITEPAL;
    DMA[3].cnt = SPRITES_PALETTE_LENGTH | DMA_ON;
    DMA[3].src = sprites;
    DMA[3].dst = &CHARBLOCKBASE[5];
    DMA[3].cnt = SPRITES_LENGTH | DMA_ON;
    ship = &(shadow[0]);
    selector = &(shadow[1]);
    enemy0 = &(shadow[2]);
    enemy1 = &(shadow[3]);
    enemy2 = &(shadow[4]);
    enemy3 = &(shadow[5]);
    enemy4 = &(shadow[6]);
    pause = &(shadow[7]);

    for(int i = 0; i < 128; i++) {
       shadow[i].attr0 = ATTR0_HIDE;
   }

}

void drawSprites(void) {
    DMA[3].src = shadow;
    DMA[3].dst = OAMMEM;
    DMA[3].cnt = 128*4 | DMA_ON;
}

void drawEnemy0(int x, int y) {
    enemy0->attr2 = ENEMY0_PALETTE_ID | ENEMY0_ID;
    enemy0->attr0 = y | SPRITES_PALETTE_TYPE | ENEMY0_SPRITE_SHAPE;
    enemy0->attr1 = x | ENEMY0_SPRITE_SIZE;
}

void hideEnemy0(void) {
    enemy0->attr0 = ATTR0_HIDE;
}

void drawEnemy1(int x, int y) {
    enemy1->attr2 = ENEMY1_PALETTE_ID | ENEMY1_ID;
    enemy1->attr0 = y | SPRITES_PALETTE_TYPE | ENEMY1_SPRITE_SHAPE;
    enemy1->attr1 = x | ENEMY1_SPRITE_SIZE;
}

void hideEnemy1(void) {
    enemy1->attr0 = ATTR0_HIDE;
}

void drawEnemy2(int x, int y) {
    enemy2->attr2 = ENEMY2_PALETTE_ID | ENEMY2_ID;
    enemy2->attr0 = y | SPRITES_PALETTE_TYPE | ENEMY2_SPRITE_SHAPE;
    enemy2->attr1 = x | ENEMY2_SPRITE_SIZE;
}

void hideEnemy2(void) {
    enemy2->attr0 = ATTR0_HIDE;
}

void drawEnemy3(int x, int y) {
    enemy3->attr2 = ENEMY3_PALETTE_ID | ENEMY3_ID;
    enemy3->attr0 = y | SPRITES_PALETTE_TYPE | ENEMY3_SPRITE_SHAPE;
    enemy3->attr1 = x | ENEMY3_SPRITE_SIZE;
}

void hideEnemy3(void) {
    enemy3->attr0 = ATTR0_HIDE;
}

void drawEnemy4(int x, int y) {
    enemy4->attr2 = ENEMY4_PALETTE_ID | ENEMY4_ID;
    enemy4->attr0 = y | SPRITES_PALETTE_TYPE | ENEMY4_SPRITE_SHAPE;
    enemy4->attr1 = x | ENEMY4_SPRITE_SIZE;
}

void hideEnemy4(void) {
    enemy4->attr0 = ATTR0_HIDE;
}

void drawShip(int x, int y) {
    ship->attr2 = SHIP_PALETTE_ID | SHIP_ID;
    ship->attr0 = y | SPRITES_PALETTE_TYPE | SHIP_SPRITE_SHAPE;
    ship->attr1 = x | SHIP_SPRITE_SIZE;
}
void hideShip(void) {
    ship->attr0 = ATTR0_HIDE;
}

void drawSelector(int x, int y) {
    selector->attr2 = SELECTOR_PALETTE_ID | SELECTOR_ID;
    selector->attr0 = y | SPRITES_PALETTE_TYPE | SELECTOR_SPRITE_SHAPE;
    selector->attr1 = x | SELECTOR_SPRITE_SIZE;
}

void hideSelector(void) {
    selector->attr0 = ATTR0_HIDE;
}

void drawPause(void) {
    pause->attr2 = PAUSE_PALETTE_ID | PAUSE_ID;
    pause->attr0 = 47 | SPRITES_PALETTE_TYPE | PAUSE_SPRITE_SHAPE;
    pause->attr1 = 87 | PAUSE_SPRITE_SIZE;
}

void hidePause(void) {
    pause->attr0 = ATTR0_HIDE;
}

void delay(int n) {
    while(vBlankCounter % n != 0) {
        waitForVBlank();
    }
}

void drawMissile(int x, int y, volatile u16 color) {
    int location = OFFSET(y, x, 240);
    DMA[3].cnt = 0;
    DMA[3].src = &color;
    DMA[3].dst = &(videoBuffer[location]);
    DMA[3].cnt = 2 | DMA_ON | DMA_SOURCE_FIXED;
    y = y + 1;
    location = OFFSET(y, x, 240);
    DMA[3].cnt = 0;
    DMA[3].src = &color;
    DMA[3].dst = &(videoBuffer[location]);
    DMA[3].cnt = 2 | DMA_ON | DMA_SOURCE_FIXED;
}

void undrawMissile(int x, int y, const u16 *image) {
    DMA[3].cnt = 0;
    y = y + 4;
    DMA[3].src = &(image[OFFSET(y, x, 240)]);
    DMA[3].dst = &(videoBuffer[OFFSET(y, x, 240)]);
    DMA[3].cnt = 2 | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT | DMA_ON;
    y = y + 1;
    DMA[3].src = &(image[OFFSET(y, x, 240)]);
    DMA[3].dst = &(videoBuffer[OFFSET(y, x, 240)]);
    DMA[3].cnt = 2 | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT | DMA_ON;
}

void clearMissile(int x, const u16 *image) {
    DMA[3].cnt = 0;
    for (int y = 5; y >= 0; y--) {
        DMA[3].src = &(image[OFFSET(y, x, 240)]);
        DMA[3].dst = &(videoBuffer[OFFSET(y, x, 240)]);
        DMA[3].cnt = 2 | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT | DMA_ON;
    }
}
