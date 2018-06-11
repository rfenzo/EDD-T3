#include "state.h"
#include "box.h"
#include "stdlib.h"

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

State* stateInit(Box*** tablero, State* parent, Box* op, unsigned int key){
  State* state = malloc(sizeof(State));
  state->tablero = tablero;
  state->parent = parent;
  state->op = op;
  state->key = intToChars(key);
  return state;
}
