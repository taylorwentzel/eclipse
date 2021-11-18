#ifndef LOGIC_H
#define LOGIC_H

#include "gba.h"

typedef struct {
    volatile int mX;
    volatile int mY;
} Missile;

typedef struct {
    volatile int enemyX;
    volatile int enemyY;
    volatile int dead;
} Enemy;



typedef struct {
    // Store whether or not the game is over in this member:
    volatile int gameOver;
    volatile int score;
    volatile int lives;
    volatile int shipX;
    volatile int shipY;  //to change speed of ship, add a counter
    volatile Enemy baddies[50];
    volatile int noMissiles;
    volatile Missile shots[NO_MISSILES];
    volatile int justDied;

} AppState;

// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);

AppState processDeath(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);

#endif
