#pragma once
#include "state.h"

struct dictEntry {
    const char *key;
    State* value;
};
typedef struct dictEntry DictEntry;

struct dict {
    int len;
    int cap;
    DictEntry* entry;
};
typedef struct dict Dict;

State* dictFind(Dict* dict, const char *key);

void dictAdd(Dict* dict, const char *key, State* value);

Dict* dictInit(void);
