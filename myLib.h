//sprites
#define OBJ_ENABLE (1<<12)

typedef struct {
   unsigned short attr0;
   unsigned short attr1;
   unsigned short attr2;
   unsigned short fill;
} OamEntry;

#define OAMMEM  ((OamEntry*)0x7000000)
#define SPRITEPAL ((unsigned short *)0x5000200)

typedef struct {
    unsigned short tileimg[8192];
} charblock;

#define CHARBLOCKBASE ((charblock*)0x6000000)
#define ATTR0_REG (0<<8) // Default
#define ATTR0_HIDE (2<<8) // If set the sprite is hidden, by default all sprites are SHOWN
#define ATTR0_MOSAIC (1<<12) // C controls Mosaic effect if set the sprite will appear pixelated.
#define ATTR1_NOFLIP 0
#define ATTR1_HFLIP (1<<12)
#define ATTR1_VFLIP (1<<13)
#define PRIORITY(pri) ((pri) << 10)

//rooty shooty
#define NO_MISSILES 100

//ship

extern void initializeSprites(void);
extern void drawSprites(void);
extern void drawShip(int x, int y);
extern void drawSelector(int x, int y);
extern void hideSelector(void);
extern void delay(int n);
extern void drawEnemy0(int x, int y);
extern void drawEnemy1(int x, int y);
extern void drawEnemy2(int x, int y);
extern void drawEnemy3(int x, int y);
extern void drawEnemy4(int x, int y);
extern void drawMissile(int x, int y, unsigned short color);
extern void undrawMissile(int x, int y, const unsigned short *image);
extern void clearMissile(int x, const unsigned short *image);
extern void undrawScore(int x, int y, volatile unsigned short color);
extern void drawPause(void);
extern void hideShip(void);
extern void hidePause(void);
extern void undrawLives(volatile unsigned short color);
extern void drawDeathScreen(const unsigned short *image);
extern void hideEnemy0(void);
extern void hideEnemy1(void);
extern void hideEnemy2(void);
extern void hideEnemy3(void);
extern void hideEnemy4(void);