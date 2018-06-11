#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "box.h"

Box* boxInit(int value, int col, int row){
  Box* box = malloc(sizeof(Box));
  box->value = value;
  box->row = row;
  box->col = col;
  return box;
}

void displayBox(Box* box){
  printf("(%i,%i): ", box->col, box->row);
  if (box->value == 5) {
    printf("Empty\n");
  }else{
    if (box->value < 4) {
      printf("Aliado apuntando hacia ");
      if (box->value == 0) {
        printf("arriba\n");
      }else if (box->value == 1) {
        printf("derecha\n");
      }else if (box->value == 2) {
        printf("abajo\n");
      }else if (box->value == 0) {
        printf("izquierda\n");
      }
    }else{
      printf("Enemigo\n");
    }
  }
}
