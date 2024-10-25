#ifndef HASHMAP_H
#define HASHMAP_H
#include <stdio.h>
#include <stdint.h>

typedef struct hashmap hashmap;

hashmap *hashmap_init(uint32_t init_cap, uint32_t max_cap, double load_factor);
void hashmap_put(hashmap *self, char *key, int value);
void hashmap_remove(hashmap *self, char *key);
int hashmap_get(hashmap *self, char *key);
void hashmap_destroy(hashmap *self);
size_t hashmap_size(hashmap *self);

#endif
