#ifndef MENU_A
#define MENU_A

#include <string>
#include <vector>

#include "graphics.hpp"

extern const Label aMenuButtons[6];
extern const Label aMenuEmptySlotButtons[3];
extern const Label aMenuTopBarButtons[6];

// Shows the A menu
int aMenu(int pkmX, int pkmY, const Label *buttons, int buttonsCount, int buttonMode);

#endif
