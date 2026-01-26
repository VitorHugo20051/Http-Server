#include <stdlib.h>
#include <string.h>

#include "Entry.h"

struct Entry entry_constructor(void *key, unsigned long key_size, void *value, unsigned long value_size) {
    struct Entry entry;
    entry.key = key;
    entry.value = value;
    memcpy(entry.key, key, key_size);
    memcpy(entry.value, value, value_size);

    return entry;
}

void entry_destructor(struct Entry *entry) {
    free(entry->key);
    free(entry->value);
    free(entry);
}