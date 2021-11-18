#include "draw.h"
#include "gba.h"
#include "gameBackground.h"
#include "deathScreen.h"
#include <stdio.h>
#include <stdlib.h>

void fullDrawAppState(AppState *state) {
    hideSelector();
    drawFullScreenImageDMA(gameBackground);
    drawShip(state->shipX, state->shipY);
    drawSprites();
    char buffer[51];
    sprintf(buffer, "Score: %d", state->score);
    drawString(1, 150, buffer, WHITE);
    char buff[51];
    sprintf(buff, "Lives: %d", state->lives);
    drawString(190, 150, buff, WHITE);
}

void undrawAppState(AppState *state, AppState *next) {
    if (state->score != next->score) {
        undrawScore(1, 150, BLACK);
    }
    if (state->lives != next->lives) {
        undrawLives(BLACK);
    }
}

void drawAppState(AppState *curr, AppState *state) {

    for (int i = 0; i < NO_MISSILES; i++) {
        if (state->shots[i].mY < 155 && state->shots[i].mY > 0) {
            drawMissile(state->shots[i].mX, state->shots[i].mY, YELLOW);
            if (state->gameOver) {
                undrawMissile(state->shots[i].mX, state->shots[i].mY, deathScreen);
            } else {
                undrawMissile(state->shots[i].mX, state->shots[i].mY, gameBackground);
            }
        } else {
            clearMissile(state->shots[i].mX, gameBackground);
        }
    }

    //draw score
    if (curr->score != state->score || state->gameOver) {
        char buffer[51];
        sprintf(buffer, "Score: %d", state->score);
        drawString(1, 150, buffer, WHITE);
    }

    //draw lives
    if (curr->lives != state->lives || state->gameOver) {
        char buff[51];
        sprintf(buff, "Lives: %d", state->lives);
        drawString(190, 150, buff, WHITE);
    }

    if (state->justDied || state->gameOver) {
        hideShip();
    } else {
        drawShip(state->shipX, state->shipY);
    }

    drawEnemy0(state->baddies[0].enemyX, state->baddies[0].enemyY);
    drawEnemy1(state->baddies[1].enemyX, state->baddies[1].enemyY);
    drawEnemy2(state->baddies[2].enemyX, state->baddies[2].enemyY);
    drawEnemy3(state->baddies[3].enemyX, state->baddies[3].enemyY);
    drawEnemy4(state->baddies[4].enemyX, state->baddies[4].enemyY);
    drawSprites();
    delay(1);

}
