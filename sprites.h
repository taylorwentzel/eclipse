#ifndef SPRITES_H
#define SPRITES_H

#define SPRITES_PALETTE_TYPE (0 << 13)
#define SPRITES_DIMENSION_TYPE (1 << 6)

extern const unsigned short sprites_palette[256];
#define SPRITES_PALETTE_SIZE 512
#define SPRITES_PALETTE_LENGTH 256

extern const unsigned short sprites[2576];
#define SPRITES_SIZE 5152
#define SPRITES_LENGTH 2576

#define SHIP_PALETTE_ID (0 << 12)
#define SHIP_SPRITE_SHAPE (0 << 14)
#define SHIP_SPRITE_SIZE (2 << 14)
#define SHIP_ID 512

#define ENEMY0_PALETTE_ID (0 << 12)
#define ENEMY0_SPRITE_SHAPE (0 << 14)
#define ENEMY0_SPRITE_SIZE (2 << 14)
#define ENEMY0_ID 528

#define ENEMY1_PALETTE_ID (0 << 12)
#define ENEMY1_SPRITE_SHAPE (0 << 14)
#define ENEMY1_SPRITE_SIZE (2 << 14)
#define ENEMY1_ID 544

#define ENEMY2_PALETTE_ID (0 << 12)
#define ENEMY2_SPRITE_SHAPE (0 << 14)
#define ENEMY2_SPRITE_SIZE (2 << 14)
#define ENEMY2_ID 560

#define ENEMY3_PALETTE_ID (0 << 12)
#define ENEMY3_SPRITE_SHAPE (0 << 14)
#define ENEMY3_SPRITE_SIZE (2 << 14)
#define ENEMY3_ID 576

#define ENEMY4_PALETTE_ID (0 << 12)
#define ENEMY4_SPRITE_SHAPE (0 << 14)
#define ENEMY4_SPRITE_SIZE (2 << 14)
#define ENEMY4_ID 592

#define PAUSE_PALETTE_ID (0 << 12)
#define PAUSE_SPRITE_SHAPE (0 << 14)
#define PAUSE_SPRITE_SIZE (3 << 14)
#define PAUSE_ID 608

#define SELECTOR_PALETTE_ID (0 << 12)
#define SELECTOR_SPRITE_SHAPE (0 << 14)
#define SELECTOR_SPRITE_SIZE (0 << 14)
#define SELECTOR_ID 672

#endif

