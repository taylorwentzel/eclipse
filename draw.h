#ifndef DRAW_H
#define DRAW_H

#include "logic.h"

void fullDrawAppState(AppState *state);
void undrawAppState(AppState *state, AppState *next);
void drawAppState(AppState *curr, AppState *state);

#endif
