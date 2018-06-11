#include "state.h"
#include "box.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

char* intToChars(unsigned int numero) {
  char* result = calloc(4,1);
  for (int j = 0; j < 4; j++) {
    if (numero != 0) {
      for (int i = 0; i < 8; i++) {
        if (numero % 2 == 0) {
          result[4-1-j] = result[4-1-j] | 0 << i;
        }else{
          result[4-1-j] = result[4-1-j] | 1 << i;
        }
        numero /= 2;
      }
    }else{
      break;
    }
  }
  return result;
}

State* stateInit(int* tablero,  unsigned int key, int size){
  State* state = malloc(sizeof(State));
  state->tablero = malloc(size*sizeof(int));
  memcpy(state->tablero,tablero,size*sizeof(int));
  state->key = key;
  // state->parent = malloc(sizeof(State));
  state->parent = NULL;
  // state->key = intToChars(key);
  return state;
}

void displayState(State state, int width, int height){
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      printf("|%i|", state.tablero[row*width+col]);
      // displayBox(state->tablero[row*width+col]);
    }
    printf("\n");
  }
  printf("\n");
}
