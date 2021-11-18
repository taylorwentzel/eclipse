#include "logic.h"

void initializeAppState(AppState* appState) {
    appState->gameOver = 0;
    appState->score = 0;
    appState->lives = 3;
    appState->shipX = 104;
    appState->shipY = 125;
    appState->noMissiles = 0;
    appState->justDied = 0;

    int startPosition = 10;

    for (int i = 0; i < 5; i++) {
        appState->baddies[i].enemyX = startPosition;
        appState->baddies[i].enemyY = 224;
        appState->baddies[i].dead = 0;
        startPosition = startPosition + 50;
    }

    //initilaize the missiles
    int mXStart = 0;
    int mYStart = 155;
    for (int i = 0; i < NO_MISSILES; i++) {
        appState->shots[i].mX = mXStart;
        appState->shots[i].mY = mYStart;
    }

}

AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {

    AppState nextAppState = *currentAppState;

    //Ship's movement controls
    if (KEY_DOWN(BUTTON_DOWN, keysPressedNow) && KEY_DOWN(BUTTON_LEFT, keysPressedNow)) {

        if (currentAppState->shipY < 128) {
            nextAppState.shipY = currentAppState->shipY + 2;
        }

        if (currentAppState->shipX > 0) {
            nextAppState.shipX = currentAppState->shipX - 2;
        }

    } else if (KEY_DOWN(BUTTON_DOWN, keysPressedNow) && KEY_DOWN(BUTTON_RIGHT, keysPressedNow)) {

        if (currentAppState->shipY < 128) {
            nextAppState.shipY = currentAppState->shipY + 2;
        }

        if (currentAppState->shipX < 216) {
            nextAppState.shipX = currentAppState->shipX + 2;
        }

    } else if (KEY_DOWN(BUTTON_UP, keysPressedNow) && KEY_DOWN(BUTTON_LEFT, keysPressedNow)) {

        if (currentAppState->shipY > 0) {
            nextAppState.shipY = currentAppState->shipY - 2;
        }

        if (currentAppState->shipX > 0) {
            nextAppState.shipX = currentAppState->shipX - 2;
        }

    } else if (KEY_DOWN(BUTTON_UP, keysPressedNow) && KEY_DOWN(BUTTON_RIGHT, keysPressedNow)) {

        if (currentAppState->shipY > 0) {
            nextAppState.shipY = currentAppState->shipY - 2;
        }

        if (currentAppState->shipX < 216) {
            nextAppState.shipX = currentAppState->shipX + 2;
        }

    } else if (KEY_DOWN(BUTTON_DOWN, keysPressedNow)){

        if (currentAppState->shipY < 128) {
            nextAppState.shipY = currentAppState->shipY + 2;
        }

    } else if (KEY_DOWN(BUTTON_UP, keysPressedNow)) {

        if (currentAppState->shipY > 0) {
            nextAppState.shipY = currentAppState->shipY - 2;
        }

    } else if (KEY_DOWN(BUTTON_LEFT, keysPressedNow)) {

        if (currentAppState->shipX > 0) {
            nextAppState.shipX = currentAppState->shipX - 2;
        }

    } else if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow)) {

        if (currentAppState->shipX < 216) {
            nextAppState.shipX = currentAppState->shipX + 2;
        }

    }

    if (nextAppState.shipY < 0) {
        nextAppState.shipY = 0;
    }

    if (KEY_DOWN(BUTTON_A, keysPressedNow) && !KEY_DOWN(BUTTON_A, keysPressedBefore)) {
        nextAppState.shots[nextAppState.noMissiles].mX = currentAppState->shipX + 11;
        nextAppState.shots[nextAppState.noMissiles].mY = currentAppState->shipY - 1;
        nextAppState.noMissiles = (nextAppState.noMissiles + 1) % NO_MISSILES;
    }

    //change y coordinate of missile
    for (int i = 0; i < NO_MISSILES; i++) {
        if (nextAppState.shots[i].mY < 155  && nextAppState.shots[i].mY != 0) {
            nextAppState.shots[i].mY = nextAppState.shots[i].mY - 2;
        } else {
            nextAppState.shots[i].mY = 155;
        }
    }

    //change y coordinat of enemies
    for (int n = 0; n < 5; n++) {
        if (currentAppState->score <= 20) {
            nextAppState.baddies[n].enemyY = ((nextAppState.baddies[n].enemyY + 1) % 256);
        } else if (currentAppState->score <= 40) {
            nextAppState.baddies[n].enemyY = ((nextAppState.baddies[n].enemyY + 2) % 256);
        } else if (currentAppState->score <= 60) {
            nextAppState.baddies[n].enemyY = ((nextAppState.baddies[n].enemyY + 3) % 256);
        } else {
            nextAppState.baddies[n].enemyY = ((nextAppState.baddies[n].enemyY + 4) % 256);
        }
    }

    //enemy hit detection
    int cond1;
    int cond2;
    int cond3;
    int cond4;
    for (int i = 0; i < NO_MISSILES; i++) {
        for (int n = 0; n < 5; n++) {

            cond1 = nextAppState.shots[i].mY >= nextAppState.baddies[n].enemyY;
            cond2 = nextAppState.shots[i].mY <= nextAppState.baddies[n].enemyY + 27;
            cond3 = nextAppState.shots[i].mX >= nextAppState.baddies[n].enemyX;
            cond4 = nextAppState.shots[i].mX <= nextAppState.baddies[n].enemyX + 23;
            if (cond1 && cond2 && cond3 && cond4 && nextAppState.shots[i].mY < 155) {
                nextAppState.baddies[n].enemyY = 200;
                nextAppState.baddies[n].enemyX = randint(0, 215);
                nextAppState.score++;
            }
        }
    }

    // ship hit detection
    for (int n = 0; n < 5; n++) {
        cond1 = nextAppState.shipX >= nextAppState.baddies[n].enemyX - 22;
        cond2 = nextAppState.shipX <= nextAppState.baddies[n].enemyX + 22;
        cond3 = nextAppState.shipY <= nextAppState.baddies[n].enemyY + 26;
        cond4 = nextAppState.shipY >= nextAppState.baddies[n].enemyY - 30;
        if (cond1 && cond2 && cond3 && cond4) {
                nextAppState.shipX = 104;
                nextAppState.shipY = 125;
                if (currentAppState->lives - 1 == 0) {
                    nextAppState.gameOver = 1;
                    nextAppState.lives = 0;
                } else if (!currentAppState->justDied) {
                    nextAppState.lives--;
                }
                nextAppState.justDied = 1;
            }
    }

    UNUSED(keysPressedBefore);

    return nextAppState;
}

AppState processDeath(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    AppState nextAppState = *currentAppState;
    //change y coordinate of missile
    for (int i = 0; i < NO_MISSILES; i++) {
        if (nextAppState.shots[i].mY < 155  && nextAppState.shots[i].mY != 0) {
            nextAppState.shots[i].mY = nextAppState.shots[i].mY - 2;
        } else {
            nextAppState.shots[i].mY = 155;
        }
    }

    //change y coordinat of enemies
    nextAppState.justDied = 0;
    for (int n = 0; n < 5; n++) {
        if ((nextAppState.baddies[n].enemyY < 223 || nextAppState.baddies[n].enemyY > 229) && currentAppState->score <= 20) {
            nextAppState.baddies[n].enemyY = ((nextAppState.baddies[n].enemyY + 1) % 256);
            nextAppState.justDied = 1;
        } else if ((nextAppState.baddies[n].enemyY < 223 || nextAppState.baddies[n].enemyY > 229) && currentAppState->score <= 40) {
            nextAppState.baddies[n].enemyY = ((nextAppState.baddies[n].enemyY + 2) % 256);
            nextAppState.justDied = 1;
        } else if ((nextAppState.baddies[n].enemyY < 223 || nextAppState.baddies[n].enemyY > 229) && currentAppState->score <= 40) {
            nextAppState.baddies[n].enemyY = ((nextAppState.baddies[n].enemyY + 3) % 256);
            nextAppState.justDied = 1;
        } else if ((nextAppState.baddies[n].enemyY < 223 || nextAppState.baddies[n].enemyY > 229) && currentAppState->score > 60) {
            nextAppState.baddies[n].enemyY = ((nextAppState.baddies[n].enemyY + 4) % 256);
            nextAppState.justDied = 1;
        }
    }

    UNUSED(keysPressedBefore);
    UNUSED(keysPressedNow);
    UNUSED(currentAppState);
    return nextAppState;
}
