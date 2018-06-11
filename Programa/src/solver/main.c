#include <stdio.h>
#include <stdlib.h>
#include "random.h"
#include "box.h"
#include "state.h"
#include "dict.h"
#include "unistd.h"
#include "stdbool.h"
#include "string.h"

int HEIGHT, WIDTH;

int getPos(int col, int row){
	return row*WIDTH+col;
}

int getRow(int pos){
	return pos/WIDTH;
}

int getCol(int pos){
	return pos%WIDTH;
}

void displaySolution(State* currentState){
	int solInversa[10000];
	//WARNING de que tamaño debe ser?
	State* actual = currentState;
	int i;
	for (i = 0; actual->parent != NULL; i++) {
		solInversa[i] = actual->posOp;
		actual = actual->parent;
	}
	i--;
	for (; i >= 0; i--) {
		printf("%i,%i\n", getCol(solInversa[i]),getRow(solInversa[i]));
	}
}

bool compareState(State* currentState, State* endState){
	int pos;
	for (int row = 0; row < HEIGHT; row++) {
		for (int col = 0; col < WIDTH; col++) {
			pos = getPos(col,row);
			// printf("compareState\n");
			// displayBox(currentState->tablero[pos]);
			// displayBox(endState->tablero[pos]);
			//aliado y (enemigo o vacio)
			if (currentState->tablero[pos] < 4 && endState->tablero[pos] >= 4) {
				return false;
			}
			//enemigo y vacio
			if (currentState->tablero[pos] == 4 && endState->tablero[pos] == 5) {
				return false;
			}
		}
	}
	displaySolution(currentState);
	return true;
}

int rotar(int pos){
	return (++pos)%4;
}

int destroy(int pos){
	return 5;
}

bool getShot(int col, int row, int* tablero){
	int value = tablero[getPos(col,row)];
	if (value == 5) {
		return false;
	}
	if (value < 4) {
		// printf("Rotando %i,%i: %i->", col,row,value);
		tablero[getPos(col,row)] = rotar(value);
		// printf("%i\n", tablero[getPos(col,row)]);
	}else if (value == 4) {
		// printf("Eliminando %i,%i\n", col,row);
		tablero[getPos(col,row)] = destroy(value);
	}
	return true;
}

State* disparar(State* current, int posAliado, unsigned int newKey){
	int alCol = getCol(posAliado);
	int alRow = getRow(posAliado);
	int alValue = current->tablero[posAliado];
	// printf("Dispara %i,%i en direccion %i\n",alCol,alRow,alValue);
	// sleep(1);
	State* new = stateInit(current->tablero, newKey,HEIGHT*WIDTH);
	if (alValue == 0 && alCol-1 >= 0) {
		// revisar hacia arriba
		for (int i = alRow-1; i >= 0; i--) {
			if (getShot(alCol, i, new->tablero)){
				break;
			}
		}
	}else if (alValue == 1 && alCol+1 < WIDTH) {
		// revisar hacia la derecha
		for (int i = alCol+1; i < WIDTH; i++) {
			if (getShot(i, alRow, new->tablero)){
				break;
			}
		}
	}else if (alValue == 2 && alRow+1 < HEIGHT) {
		// revisar hacia abajo
		for (int i = alRow+1; i < HEIGHT; i++) {
			if (getShot(alCol,i, new->tablero)){
				break;
			}
		}
	}else if (alValue == 3 && alRow-1 >= 0) {
		// revisar hacia la izquierda
		for (int i = alCol-1; i >= 0; i--) {
			if (getShot(i,alRow, new->tablero)){
				break;
			}
		}
	}
	return new;
}

bool BFS(State* current, State* end, int* aliados, int aliadosCount){
	State* open[10000]; //de que tamaño?
	//Dict* stored = dictInit();
	State* new;
	int newKey = 2;
	int openCount = 0;
	int opened = 0;
	open[openCount++] = current; //dictAdd(stored, current->key, current);
	while (openCount != opened){
		// sleep(1);
		// printf("openCount %i, opened %i\n", openCount, opened);
		current = open[opened++];
		// displayState(*current, WIDTH, HEIGHT);
		for (int a = 0; a < aliadosCount; a++) {
			new = disparar(current, aliados[a], newKey++);
			//if (dictFind(stored, new->key)) continue;
			new->parent = current; new->posOp = aliados[a];
			if (compareState(new, end)) return true;
			open[openCount++] = new; //dictAdd(stored, new->key, new);
		}
	}
	return false;
}


int main(int argc, char** argv){
	if (argc != 2){
		printf("Modo de uso: ./solver test.txt\n");
		return 0;
	}
	FILE* input_file = fopen((char *)argv[1],"r");
	if (!input_file) {
		printf("Error al abrir el archivo\n");
		return 1;
	}
	fscanf(input_file, "%d %d", &WIDTH, &HEIGHT);

	int* initial = malloc(WIDTH*HEIGHT*sizeof(int));
	int* end = malloc(WIDTH*HEIGHT*sizeof(int));
	int* aliados = malloc(WIDTH*HEIGHT*sizeof(int)); // position of allys
	int aliadosCount = 0;

	int value;
	for (int row = 0; row < HEIGHT; row++) {
		for (int col = 0; col < WIDTH; col++) {
			fscanf(input_file, "%d", &value);
			initial[getPos(col,row)] = value;
			if (value < 4) {
				aliados[aliadosCount] = getPos(col,row);
				aliadosCount++;
			}
			if (value == 4) {
				end[getPos(col,row)] = 5;
			}else{
				end[getPos(col,row)] = value;
			}
		}
	}

	BFS(stateInit(initial,0,HEIGHT*WIDTH), stateInit(end,1,HEIGHT*WIDTH), aliados, aliadosCount);

	return 0;
}
