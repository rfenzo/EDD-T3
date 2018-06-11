#include "dict.h"
#include "string.h"
#include "stdlib.h"
#include "state.h"

int dictFindIndex(Dict* dict, const char *key) {
    for (int i = 0; i < dict->len; i++) {
        if (!strcmp(dict->entry[i].key, key)) {
            return i;
        }
    }
    return -1;
}

State* dictFind(Dict* dict, const char *key) {
    int idx = dictFindIndex(dict, key);
    return idx == -1 ? NULL : dict->entry[idx].value;
}

void dictAdd(Dict* dict, const char *key, State* value) {
   int idx = dictFindIndex(dict, key);
   if (idx != -1) {
       dict->entry[idx].value = value;
       return;
   }
   if (dict->len == dict->cap) {
       dict->cap *= 2;
       dict->entry = realloc(dict->entry, dict->cap * sizeof(DictEntry));
   }
   dict->entry[dict->len].key = strdup(key);
   dict->entry[dict->len].value = value;
   dict->len++;
}

Dict* dictInit(void) {
    Dict proto = {0, 10, malloc(10 * sizeof(DictEntry))};
    Dict* d = malloc(sizeof(Dict));
    *d = proto;
    return d;
}
