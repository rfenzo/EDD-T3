#pragma once
#include <stdbool.h>

struct box{
  bool empty, aliado;
  int direccion, value, row, col;
};
typedef struct box Box;

Box* boxInit(int value,int row, int col);

void displayBox(Box* box);
