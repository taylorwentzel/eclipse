//author: Taylor Wentzel

#include "draw.h"
#include "titleScreen.h"
#include "sprites.h"
#include "gameBackground.h"
#include "controls.h"
#include "deathScreen.h"
#include "options.h"
#include <stdio.h>
#include <stdlib.h>

// AppState enum definition
typedef enum {
    START,
    START_NODRAW,
    CONTROLS_SETUP,
    CONTROLS,
    OPTIONS_SETUP,
    OPTIONS,
    APP_INIT_GAME,
    APP,
    APP_DRAW_PAUSE,
    APP_PAUSE,
    APP_EXIT,
    APP_EXIT_NODRAW,
} GBAState;

int selectX = 91;
int selectY = 94;
int selectCurr = 0;

int main(void) {
    REG_DISPCNT= MODE3 | BG2_ENABLE | OBJ_ENABLE | SPRITES_DIMENSION_TYPE;

    GBAState state = START;

    // We store the "previous" and "current" states.
    AppState currentAppState, nextAppState;

    // We store the current and previous values of the button input.
    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;

    while (1) {
        // Load the current state of the buttons
        currentButtons = BUTTONS;

        switch(state) {
            case START:
                // Wait for VBlank
                waitForVBlank();
                drawFullScreenImageDMA(titleScreen);
                initializeSprites();
                selectX = 91;
                selectY = 94;
                selectCurr = 0;
                drawSelector(selectX, selectY);
                drawSprites();

                state = START_NODRAW;

                break;
            case START_NODRAW:

                if (KEY_DOWN(BUTTON_DOWN, currentButtons)) { //this statement will move the selector up and down

                    if (selectY < 108) {
                        selectY = selectY + 7;
                        waitForVBlank();
                        drawSelector(selectX, selectY);
                        drawSprites();

                        if (selectCurr == 0) {
                            selectCurr = 1;
                        } else {
                            selectCurr = 2;
                        }

                    } else {
                        selectY = 94;
                        waitForVBlank();
                        drawSelector(selectX, selectY);
                        drawSprites();

                        selectCurr = 0;

                    }
                } else if (KEY_DOWN(BUTTON_UP, currentButtons)) {

                    if (selectY > 94) {
                        selectY = selectY - 7;
                        waitForVBlank();
                        drawSelector(selectX, selectY);
                        drawSprites();

                        if (selectCurr == 2) {
                            selectCurr = 1;
                        } else {
                            selectCurr = 0;
                        }

                    } else {
                        selectY = 108;
                        waitForVBlank();
                        drawSelector(selectX, selectY);
                        drawSprites();
                        selectCurr = 2;
                    }
                } else if (KEY_DOWN(BUTTON_START, currentButtons) && !KEY_DOWN(BUTTON_START, previousButtons)) { //add in options and controls here
                    if (selectCurr == 0) {
                        state = APP_INIT_GAME;
                    } else if (selectCurr == 1) {
                        state = CONTROLS_SETUP;
                    } else if (selectCurr == 2) {
                        state = OPTIONS_SETUP;
                    }
                }

                delay(30);

                break;
            case APP_INIT_GAME:
                // Initialize the app. Switch to the APP state.
                initializeAppState(&currentAppState);

                // Draw the initial state of the app
                fullDrawAppState(&currentAppState);

                state = APP;
                break;
            case CONTROLS_SETUP:
                waitForVBlank();
                drawFullScreenImageDMA(controls);
                hideSelector();
                drawSprites();
                state = CONTROLS;
                break;
            case CONTROLS:
                if (KEY_DOWN(BUTTON_START, currentButtons) && !KEY_DOWN(BUTTON_START, previousButtons)) {
                    state = START;
                } else if ((KEY_DOWN(BUTTON_SELECT, currentButtons))) {
                    state = START;
                }
                break;
            case OPTIONS_SETUP:
                waitForVBlank();
                drawFullScreenImageDMA(optionsScreen);
                hideSelector();
                drawSprites();
                state = OPTIONS;
                break;
            case OPTIONS:
                if (KEY_DOWN(BUTTON_START, currentButtons) && !KEY_DOWN(BUTTON_START, previousButtons)) {
                    state = START;
                }  else if ((KEY_DOWN(BUTTON_SELECT, currentButtons))) {
                    state = START;
                }
                break;
            case APP:
                // Process the app for one frame, store the next state
                if (KEY_DOWN(BUTTON_B, currentButtons)) {
                    state = APP_DRAW_PAUSE;
                } else if ((KEY_DOWN(BUTTON_SELECT, currentButtons))) {
                    state = START;
                } else {
                    if (currentAppState.gameOver){
                        state = APP_EXIT;
                    } else if (currentAppState.justDied) {
                        nextAppState = processDeath(&currentAppState, previousButtons, currentButtons);
                        waitForVBlank();
                        undrawAppState(&currentAppState, &nextAppState);
                        drawAppState(&currentAppState, &nextAppState);
                        currentAppState = nextAppState;
                    } else {
                        nextAppState = processAppState(&currentAppState, previousButtons, currentButtons);

                        waitForVBlank();

                        undrawAppState(&currentAppState, &nextAppState);

                        drawAppState(&currentAppState, &nextAppState);

                        currentAppState = nextAppState;
                    }
                }

                // Check if the app is exiting. If it is, then go to the exit state.
                //if (nextAppState.gameOver) {
                    //state = APP_EXIT;
                //}

                break;
            case APP_DRAW_PAUSE:
                waitForVBlank();
                hideShip();
                hideEnemy0();
                hideEnemy1();
                hideEnemy2();
                hideEnemy3();
                hideEnemy4();
                drawSelector(96, 74);
                drawPause();
                drawSprites();
                selectX = 96;
                selectY = 74;
                selectCurr = 0;
                state = APP_PAUSE;
                break;

            case APP_PAUSE:

                if (KEY_DOWN(BUTTON_START, currentButtons) && selectCurr == 0) {
                    waitForVBlank();
                    state = APP;
                    hideSelector();
                    hidePause();

                } else if (KEY_DOWN(BUTTON_START, currentButtons) && selectCurr == 1) {
                    waitForVBlank();
                    state = START;
                    hideSelector();
                    hidePause();

                } else if (KEY_DOWN(BUTTON_DOWN, currentButtons) && selectCurr == 0) {
                    waitForVBlank();
                    selectY = 86;
                    selectCurr = 1;
                    drawSelector(selectX, selectY);
                    drawSprites();

                } else if (KEY_DOWN(BUTTON_UP, currentButtons) && selectCurr == 0) {
                    waitForVBlank();
                    selectY = 86;
                    selectCurr = 1;
                    drawSelector(selectX, selectY);
                    drawSprites();

                } else if (KEY_DOWN(BUTTON_DOWN, currentButtons) && selectCurr == 1) {
                    waitForVBlank();
                    selectY = 74;
                    selectCurr = 0;
                    drawSelector(selectX, selectY);
                    drawSprites();

                } else if (KEY_DOWN(BUTTON_UP, currentButtons) && selectCurr == 1) {
                    waitForVBlank();
                    selectY = 74;
                    selectCurr = 0;
                    drawSelector(selectX, selectY);
                    drawSprites();
                }  else if ((KEY_DOWN(BUTTON_SELECT, currentButtons))) {
                    state = START;
                }
                delay(30);
            break;

            case APP_EXIT:
                waitForVBlank();
                drawDeathScreen(deathScreen);
                nextAppState = processDeath(&currentAppState, previousButtons, currentButtons);
                waitForVBlank();
                undrawAppState(&currentAppState, &nextAppState);
                selectCurr = 0;
                selectX = 80;
                selectY = 117;
                drawSelector(selectX, selectY);
                drawAppState(&currentAppState, &nextAppState);
                state = APP_EXIT_NODRAW;
                break;

            case APP_EXIT_NODRAW:

                if (KEY_DOWN(BUTTON_START, currentButtons) && selectCurr == 0) {
                    nextAppState = processDeath(&currentAppState, previousButtons, currentButtons);
                    waitForVBlank();
                    undrawAppState(&currentAppState, &nextAppState);
                    drawAppState(&currentAppState, &nextAppState);
                    currentAppState = nextAppState;
                    state = START;

                } else if (KEY_DOWN(BUTTON_START, currentButtons) && selectCurr == 1) {
                    nextAppState = processDeath(&currentAppState, previousButtons, currentButtons);
                    waitForVBlank();
                    undrawAppState(&currentAppState, &nextAppState);
                    drawAppState(&currentAppState, &nextAppState);
                    currentAppState = nextAppState;
                    state = APP_INIT_GAME;

                } else if (KEY_DOWN(BUTTON_DOWN, currentButtons) && selectCurr == 0 && !KEY_DOWN(BUTTON_DOWN, previousButtons)) {
                    nextAppState = processDeath(&currentAppState, previousButtons, currentButtons);
                    waitForVBlank();
                    undrawAppState(&currentAppState, &nextAppState);
                    selectY = 126;
                    selectCurr = 1;
                    drawSelector(selectX, selectY);
                    drawAppState(&currentAppState, &nextAppState);
                    currentAppState = nextAppState;

                } else if (KEY_DOWN(BUTTON_UP, currentButtons) && selectCurr == 0 && !KEY_DOWN(BUTTON_UP, previousButtons)) {
                    nextAppState = processDeath(&currentAppState, previousButtons, currentButtons);
                    waitForVBlank();
                    undrawAppState(&currentAppState, &nextAppState);
                    selectY = 126;
                    selectCurr = 1;
                    drawSelector(selectX, selectY);
                    drawAppState(&currentAppState, &nextAppState);
                    currentAppState = nextAppState;

                } else if (KEY_DOWN(BUTTON_DOWN, currentButtons) && selectCurr == 1 && !KEY_DOWN(BUTTON_DOWN, previousButtons)) {
                    nextAppState = processDeath(&currentAppState, previousButtons, currentButtons);
                    waitForVBlank();
                    undrawAppState(&currentAppState, &nextAppState);
                    selectY = 117;
                    selectCurr = 0;
                    drawSelector(selectX, selectY);
                    drawAppState(&currentAppState, &nextAppState);
                    currentAppState = nextAppState;

                } else if (KEY_DOWN(BUTTON_UP, currentButtons) && selectCurr == 1 && !KEY_DOWN(BUTTON_UP, previousButtons)) {
                    nextAppState = processDeath(&currentAppState, previousButtons, currentButtons);
                    waitForVBlank();
                    undrawAppState(&currentAppState, &nextAppState);
                    selectY = 117;
                    selectCurr = 0;
                    drawSelector(selectX, selectY);
                    drawAppState(&currentAppState, &nextAppState);
                    currentAppState = nextAppState;

                }  else if ((KEY_DOWN(BUTTON_SELECT, currentButtons))) {
                    state = START;
                } else {
                    nextAppState = processDeath(&currentAppState, previousButtons, currentButtons);
                    waitForVBlank();
                    undrawAppState(&currentAppState, &nextAppState);
                    drawSelector(selectX, selectY);
                    drawAppState(&currentAppState, &nextAppState);
                    currentAppState = nextAppState;
                }

                break;
        }

        // Store the current state of the buttons
        previousButtons = currentButtons;
    }

    return 0;
}
