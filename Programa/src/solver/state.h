#pragma once
#include "box.h"

struct state{
  struct state* parent;
  Box*** tablero;
  Box* op;
  char* key;
};
typedef struct state State;

State* stateInit(Box*** tablero, State* parent, Box* op, unsigned int key);
