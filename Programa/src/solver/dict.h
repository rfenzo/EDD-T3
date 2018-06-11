#pragma once
#include "state.h"

struct dictEntry {
    unsigned int key;
    State* value;
};
typedef struct dictEntry DictEntry;

struct dict {
    int len;
    int cap;
    DictEntry* entry;
};
typedef struct dict Dict;

State* dictFind(Dict* dict, unsigned int key);

void dictAdd(Dict* dict, unsigned int key, State* value);

Dict* dictInit(void);
