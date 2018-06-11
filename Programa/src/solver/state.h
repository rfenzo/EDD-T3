#pragma once
#include "box.h"

struct state{
  struct state* parent;
  int* tablero;
  int posOp; //pos del que disparó
  unsigned int key;
};
typedef struct state State;

State* stateInit(int* tablero, unsigned int key, int size);

void displayState(State state, int width, int height);
