#include "Globals.h"

Globals::Globals(void) {}
Globals::~Globals(void) {}

Globals::Gamestate Globals::state = STATE_MENU;
int Globals::nBlocks = 0;
int Globals::lives = 0;
float Globals::speed = 0.002f;