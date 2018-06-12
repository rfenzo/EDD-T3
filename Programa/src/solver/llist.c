#include "llist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Node* nodeInit(int* value, int size){
  Node* node = malloc(sizeof(Node));
  node->value = malloc(size);
  memcpy(node->value, value, size);
  node -> next = NULL;
  return node;
}

void recursiveDestroy(Node* node){
  if (node -> next){
    recursiveDestroy(node -> next);
  }
  free(node->value);
  free(node);
}

LinkedList* linkedlistInit(){
  LinkedList* list = malloc(sizeof(LinkedList));
  list -> count = 0;
  list -> first = NULL;
  list -> last = NULL;
  return list;
}

void linkedlistAppend(LinkedList* list, int* value, int size){
  Node* node = nodeInit(value, size);
  if (list -> count == 0){
    list -> first = node;
  }else{
    list -> last -> next = node;
  }
  list -> last = node;
  list -> count++;
}

bool linkedlistExists(LinkedList* list, int* value, int size){
  Node* actual = list -> first;
  for (int i = 0; i < list->count; i++){
    if (memcmp(actual->value, value, size) == 0) {
      return true;
    }
    actual = actual -> next;
  }
  return false;
}

void dictAdd(LinkedList** dict, int* value, unsigned long position, int size){
  if (dict[position] == NULL) {
    dict[position] = linkedlistInit();
  }
  linkedlistAppend(dict[position], value, size);
}

bool inDict(LinkedList** dict, int* value, unsigned long position, int size){
  LinkedList* list = dict[position];
  if (list == NULL) {
    dict[position] = linkedlistInit();
    return false;
  }
  return linkedlistExists(list, value, size);
}

LinkedList** dictInit(int DICTSIZE){
  LinkedList** dict = malloc(sizeof(LinkedList)*DICTSIZE);
  for (int i = 0; i < DICTSIZE; i++) {
    dict[i] = NULL;
  }
  return dict;
}

void linkedlistDestroy(LinkedList* list){
  if (list -> first != NULL) {
    recursiveDestroy(list -> first);
  }
  free(list);
}

void dictDestroy(LinkedList** dict, int DICTSIZE){
  for (int i = 0; i < DICTSIZE; i++) {
    if (dict[i] != NULL){
      linkedlistDestroy(dict[i]);
    }
  }
  free(dict);
}
