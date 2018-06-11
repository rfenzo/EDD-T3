#include <stdio.h>
#include <stdlib.h>
#include "random.h"
#include "box.h"
#include "state.h"
#include "dict.h"
#include "unistd.h"

int getPos(int col, int row, int width){
	return row*width+col;
}

bool compareState(State* state1, State* state2, int width, int height){
	int pos;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			pos = getPos(col,row,width);
			// printf("compareState\n");
			// displayBox(state1->tablero[pos]);
			// displayBox(state2->tablero[pos]);
			//aliado y (enemigo o vacio)
			if (state1->tablero[pos]->value < 4 && state2->tablero[pos]->value >= 4) {
				return false;
			}
			//enemigo y vacio
			if (state1->tablero[pos]->value == 4 && state2->tablero[pos]->value == 5) {
				return false;
			}
		}
	}
	return true;
}

Box* rotar(Box* aliado){
	printf("Rotate %i,%i\n",aliado->col,aliado->row);
	// printf("Antes: %i\n", aliado->value);
	return boxInit((++aliado->value)%4,aliado->col,aliado->row);
}

Box* destroy(Box* aliado){
	printf("Destroy %i,%i\n",aliado->col,aliado->row);
	return boxInit(5,aliado->col,aliado->row);
}

bool getShot(int col, int row, int width, Box** tablero){
	int pos = getPos(col,row,width);
	Box* box = tablero[pos];
	if (box->value == 5) {
		return false;
	}
	if (box->value < 4) {
		tablero[pos] = rotar(box);
	}else if (box->value == 4) {
		tablero[pos] = destroy(box);
	}
	return true;
}

State* disparar(State* current, Box* aliado, unsigned int newKey, int width, int height){
	printf("Dispara %i,%i, direccion %i\n",aliado->col,aliado->row, aliado->value);
	// sleep(1);
	// displayBox(aliado);
	State* new = stateInit(current->tablero, newKey);
	if (aliado->value == 0 && aliado->col-1 >= 0) {
		// revisar hacia arriba
		for (int i = aliado->row-1; i >= 0; i--) {
			if (getShot(aliado->col, i, width, new->tablero)){
				break;
			}
		}
	}else if (aliado->value == 1 && aliado->col+1 < width) {
		// revisar hacia la derecha
		for (int i = aliado->col+1; i < width; i++) {
			if (getShot(i, aliado->row ,width, new->tablero)){
				break;
			}
		}
	}else if (aliado->value == 2 && aliado->row+1 < height) {
		// revisar hacia abajo
		for (int i = aliado->row+1; i < height; i++) {
			if (getShot(aliado->col,i,width, new->tablero)){
				break;
			}
		}
	}else if (aliado->value == 3 && aliado->row-1 >= 0) {
		// revisar hacia la izquierda
		for (int i = aliado->col-1; i >= 0; i--) {
			if (getShot(i,aliado->row,width, new->tablero)){
				break;
			}
		}
	}
	return new;
}

bool BFS(State* current, State* end, Box** aliados, int aliadosCount, int width, int height){
	State* open[10000]; //de que tamaño?
	//Dict* stored = dictInit();
	State* new;
	int newKey = 2;
	int openCount = 0;
	int opened = 0;
	open[openCount++] = current; //dictAdd(stored, current->key, current);
	while (openCount != opened){
		sleep(1);
		// printf("openCount %i, opened %i\n", openCount, opened);
		current = open[opened++];
		displayState(current, width, height);
		for (int a = 0; a < aliadosCount; a++) {
			printf("a: %i\n", a);
			new = disparar(current, aliados[a], newKey++, width, height);
			//if (dictFind(stored, new->key)) continue;
			new->parent = current; new->op = aliados[a];
			if (compareState(new, end, width, height)) return true;
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
	//WARNING (columna,fila)  aumenta desde esquina superior izq

	int height, width, value;
	FILE* input_file = fopen((char *)argv[1],"r");
	if (!input_file) {
		printf("Error al abrir el archivo\n");
		return 1;
	}
	fscanf(input_file, "%d %d", &width, &height);

	Box** initial = malloc(width*height*sizeof(Box));
	Box** end = malloc(width*height*sizeof(Box));
	Box** aliados = malloc(width*height*sizeof(Box)); // más memoria de la necesaria pero filo
	int aliadosCount = 0;

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			fscanf(input_file, "%d", &value);
			initial[getPos(col,row,width)] = boxInit(value, col, row);
			if (value < 4) {
				// printf("Pos de aliado %i \n", getPos(col,row,width));
				aliados[aliadosCount] = initial[getPos(col,row,width)];
				aliadosCount++;
			}
			if (value == 4) {
				end[getPos(col,row,width)] = boxInit(5, col, row);
			}else{
				end[getPos(col,row,width)] = boxInit(value, col, row);
			}
		}
	}
	BFS(stateInit(initial,0), stateInit(end,1), aliados,
	 aliadosCount, width, height);

	//display solution


	return 0;
}
