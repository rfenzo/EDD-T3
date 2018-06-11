#include <stdio.h>
#include <stdlib.h>
#include "random.h"
#include "box.h"
#include "state.h"
#include "dict.h"

bool compareState(State* state1, State* state2, int width, int height){
	for (int col = 0; col < width; col++) {
		for (int row = 0; row < height; row++) {
			//aliado y (enemigo o vacio)
			if (state1->tablero[col][row]->value < 4 && state2->tablero[col][row]->value >= 4) {
				return false;
			}
			//enemigo y vacio
			if (state1->tablero[col][row]->value == 4 && state2->tablero[col][row]->value == 5) {
				return false;
			}
		}
	}
	return true;
}

Box* rotar(Box* aliado){
	Box* newBox = boxInit(aliado->value,aliado->col,aliado->row);
	newBox->direccion++;
	newBox->direccion %= 4;
	return newBox;
}

Box* destroy(Box* aliado){
	Box* newBox = boxInit(5,aliado->col,aliado->row);
	return newBox;
}

State* disparar(State* current, Box* aliado, unsigned int newKey, int width, int height){
	displayBox(aliado);
	State* new = stateInit(current->tablero, current, aliado, newKey);
	Box* box;
	if (aliado->direccion == 0 && aliado->col-1 >= 0) {
		// revisar hacia arriba
		for (int i = aliado->col-1; i >= 0; i++) {
			box = new->tablero[i][aliado->row];
			if (box->value < 4) {
				new->tablero[i][aliado->row] = rotar(box);
			}else if (box->value == 4) {
				new->tablero[i][aliado->row] = destroy(box);
			}
		}
	}else if (aliado->direccion == 1 && aliado->row+1 < width) {
		// revisar hacia la derecha
		for (int i = aliado->row+1; i < width; i++) {
			box = new->tablero[aliado->col][i];
			if (box->value < 4) {
				rotar(box);
			}else if (box->value == 4) {
				destroy(box);
			}
		}
	}else if (aliado->direccion == 2 && aliado->col+1 < height) {
		// revisar hacia abajo
		for (int i = aliado->col+1; i < height; i++) {
			box = new->tablero[i][aliado->row];
			if (box->value < 4) {
				rotar(box);
			}else if (box->value == 4) {
				destroy(box);
			}
		}
	}else if (aliado->direccion == 3 && aliado->row-1 >= 0) {
		// revisar hacia la izquierda
		for (int i = aliado->row-1; i >= 0; i--) {
			box = new->tablero[aliado->col][i];
			if (box->value < 4) {
				rotar(box);
			}else if (box->value == 4) {
				destroy(box);
			}
		}
	}
}

bool BFS(State* current, State* end, Box** aliados, int aliadosCount, int width, int height){
	State* open[10000]; //de que tamaño?
	Dict* stored = dictInit();
	State* new;
	int newKey = 2;
	int openCount = 0;
	open[openCount++] = current; dictAdd(stored, current->key, current);
	while (openCount != 0){
		current = open[openCount--];
		for (int a = 0; a < aliadosCount; a++) {
			new = disparar(current, aliados[a], newKey++, width, height);
			if (dictFind(stored, new->key)) continue;
			new->parent = current; new->op = aliados[a];
			if (current == end) return true;
		}
	}
	return false;
}

int main(int argc, char** argv){
	if (argc != 2){
		printf("Modo de uso: ./solver test.txt\n");
		return 0;
	}
	//WARNING (columna,fila)  aumenta desde esquina superior izq

	int height, width, value;
	FILE* input_file = fopen((char *)argv[1],"r");
	if (!input_file) {
		printf("Error al abrir el archivo\n");
		return 1;
	}
	fscanf(input_file, "%d %d", &width, &height);

	Box* initial[width][height];
	Box* end[width][height];
	Box* aliados[width*height]; // más memoria de la necesaria pero filo
	int aliadosCount = 0;

	for (int col = 0; col < width; col++) {
		for (int row = 0; row < height; row++) {
			fscanf(input_file, "%d", &value);
			initial[col][row] = boxInit(value, col, row);
			if (value < 4) {
				aliados[aliadosCount] = initial[col][row];
				aliadosCount++;
			}
			if (value == 4) {
				end[col][row] = boxInit(5, col, row);
			}else{
				end[col][row] = boxInit(value, col, row);
			}
		}
	}

	BFS(stateInit(initial, NULL, NULL,0), stateInit(end, NULL, NULL,1), aliados,
	 aliadosCount, width, height);

	return 0;
}
