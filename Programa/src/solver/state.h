#pragma once
#include "box.h"

struct state{
  struct state* parent;
  Box** tablero;
  Box* op;
  unsigned int key;
};
typedef struct state State;

State* stateInit(Box** tablero, unsigned int key);

void displayState(State* state, int width, int height);
