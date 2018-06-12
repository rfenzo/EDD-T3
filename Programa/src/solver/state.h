#pragma once

struct state{
  struct state* parent;
  int* tablero;
  int posOp; //pos del que disparó
};
typedef struct state State;

State* stateInit(int* tablero, int size);

void displayState(State state, int width, int height);

void displayTablero(int* tablero, int width, int height);
