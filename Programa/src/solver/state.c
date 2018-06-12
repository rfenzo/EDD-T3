#include "state.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

void displayTablero(int* tablero, int width, int height){
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      printf("%i ", tablero[row*width+col]);
    }
    printf("\n");
  }
}

void displayState(State state, int width, int height){
  displayTablero(state.tablero, width, height);
}

State* stateInit(int* tablero, int len){
  State* state = malloc(sizeof(State));
  state->tablero = malloc(len*sizeof(int));
  memcpy(state->tablero,tablero,len*sizeof(int));
  state->parent = NULL;
  return state;
}
