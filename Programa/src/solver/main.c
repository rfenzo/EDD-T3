#include <stdio.h>
#include <stdlib.h>
#include "random.h"
#include "state.h"
#include "llist.h"
#include "unistd.h"
#include "stdbool.h"
#include "string.h"

#define DICTSIZE 10000000

int HEIGHT, WIDTH;
uint64_t* zobristTable;
LinkedList** dict;

int getPos(int col, int row){
	return row*WIDTH+col;
}

int getRow(int pos){
	return pos/WIDTH;
}

int getCol(int pos){
	return pos%WIDTH;
}

void initZobrist(){
	zobristTable = malloc(sizeof(uint64_t)*5*WIDTH*HEIGHT);
	for (int i = 0; i < WIDTH*HEIGHT; i++) {
		for (int j = 0; j < 5; j++) {
			zobristTable[j*(i+1)] = get_random_number();
		}
	}
}

uint64_t hash(int* tablero){
	uint64_t h = 0;
	for (int i = 0; i < WIDTH*HEIGHT; i++){
		if (tablero[i] != 5){
			h ^= zobristTable[tablero[i]*(i+1)];
		}
	}
	return h%DICTSIZE;
}

void displaySolution(State* currentState){
	int solInversa[100];
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

bool getShot(int col, int row, State* state){
	int value = state->tablero[getPos(col,row)];
	if (value == 5) {
		return false;
	}
	if (value < 4) {
		state->tablero[getPos(col,row)] = (++value)%4;
	}else if (value == 4) {
		state->tablero[getPos(col,row)] = 5;
	}
	return true;
}

State* disparar(State* current, int posAliado){
	int alCol = getCol(posAliado);
	int alRow = getRow(posAliado);
	int alValue = current->tablero[posAliado];
	State* new = stateInit(current->tablero,HEIGHT*WIDTH);
	if (alValue == 0) {
		// revisar hacia arriba
		for (int i = alRow-1; i >= 0; i--) {
			if (getShot(alCol, i, new)){
				break;
			}
		}
	}else if (alValue == 1) {
		// revisar hacia la derecha
		for (int i = alCol+1; i < WIDTH; i++) {
			if (getShot(i, alRow, new)){
				break;
			}
		}
	}else if (alValue == 2) {
		// revisar hacia abajo
		for (int i = alRow+1; i < HEIGHT; i++) {
			if (getShot(alCol,i, new)){
				break;
			}
		}
	}else if (alValue == 3) {
		// revisar hacia la izquierda
		for (int i = alCol-1; i >= 0; i--) {
			if (getShot(i,alRow, new)){
				break;
			}
		}
	}
	return new;
}

bool checkEndStatus(State* end){
	for (int i = 0; i < HEIGHT*WIDTH; i++) {
		if (end->tablero[i] == 4) {
			return false;
		}
	}
	displaySolution(end);
	// displayState(*end, WIDTH, HEIGHT);
	return true;
}

bool BFS(State* current, int* aliados, int aliadosCount){
	State* states[1000000];
	dict = malloc(sizeof(LinkedList)*DICTSIZE);
	initZobrist();
	State* new;
	int statesCount = 0;
	int opened = 0;
	states[statesCount++] = current;
	dictAdd(dict, current->tablero, hash(current->tablero));
	while (statesCount > opened){
		current = states[opened++];
		for (int a = 0; a < aliadosCount; a++) {
			new = disparar(current, aliados[a]);
			unsigned long hashNumber = hash(new->tablero);
			if (inDict(dict, new->tablero, hashNumber, HEIGHT*WIDTH)){
				continue;
			}
			new->parent = current; new->posOp = aliados[a];
			if (checkEndStatus(new)) return true;
			states[statesCount++] = new;
			dictAdd(dict, new->tablero, hashNumber);
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
	fscanf(input_file, "%d %d", &HEIGHT, &WIDTH);

	int* initial = malloc(WIDTH*HEIGHT*sizeof(int));
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
		}
	}

	BFS(stateInit(initial,HEIGHT*WIDTH), aliados, aliadosCount);

	return 0;
}
