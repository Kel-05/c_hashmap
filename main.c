#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEF_INIT_CAP 16
#define DEF_MAX_CAP (2 << 30)

typedef struct node {
  uint32_t hash;
  char *key;
  int value;
  struct node *next;
} node;

typedef struct hashmap {
  struct node **table;
  uint32_t init_cap;
  uint32_t max_cap;

  int (*get)(struct hashmap*, char*, size_t);
  void (*put)(struct hashmap*, char*, size_t, int);
  void (*rehash)(struct hashmap *, size_t);
} hashmap;

uint32_t str_hashcode(char *key, size_t key_size) {
  uint32_t hash = 0;
  for(size_t i = 0; i < key_size; i++) {
    hash += key[i];
  }
  return hash << 5;
}

void hashmap_rehash(hashmap *self, size_t size) {
  uint32_t new_cap = self->init_cap;

  while(new_cap < size) {
    new_cap <<= 1;
  }
  
  if(new_cap > self->max_cap || new_cap < self->init_cap) {
    fprintf(stderr, "Error: hashmap_rehash: invalid capacity: %u\n", new_cap);
    abort();
  }
  
  node **new_table = (node **) malloc(sizeof(node *) * new_cap);
  memset(new_table, 0, sizeof(node *) * new_cap);

  for(uint32_t i = 0; i < self->init_cap; i++) {
    if(self->table[i] != NULL) {
      new_table[i] = self->table[i];
    }
  }

  free(self->table);
  self->table = new_table;
  self->init_cap = new_cap;
}

int hashmap_get(hashmap *self, char *key, size_t key_size) {
  uint32_t hash = str_hashcode(key, key_size);
  
  if(self->init_cap < hash + 1) {
    return -1;
  }

  node *nd = self->table[hash];

  if(nd == NULL) {
    return -1;
  }

  if(strcmp(nd->key, key) == 0) {
    return nd->value;
  }
  else {
    while(nd->next != NULL) {
      if(strcmp(nd->key, key) == 0) return nd->value;
      nd = nd->next;
    }
  }
  
  return -1;
}

void hashmap_put(hashmap *self, char *key, size_t key_size, int value) {
  uint32_t hash = str_hashcode(key, key_size);
  node *nd = (node *) malloc(sizeof(node)), *prev_nd;

  if(self->init_cap < hash + 1) {
    hashmap_rehash(self, hash + 1);
  }
  
  node *next_nd = self->table[hash];
  
  nd->hash = hash;
  nd->key = key;
  nd->value = value;
  nd->next = NULL;

  if(next_nd != NULL) {
    while(next_nd != NULL) {
      prev_nd = next_nd;
      next_nd = next_nd->next;
    }
    prev_nd->next = nd;
  }
  else {
    self->table[hash] = nd;
  }
  
}

void hashmap_init(hashmap *self, uint32_t init_cap, uint32_t max_cap) {
  self->init_cap = init_cap ? init_cap : DEF_INIT_CAP;
  self->init_cap = max_cap ? max_cap : DEF_MAX_CAP;
  
  self->table = (node **) malloc(sizeof(node *) * self->init_cap);
  memset(self->table, 0, sizeof(node *) * self->init_cap);
  
  self->get = hashmap_get;
  self->put = hashmap_put;
  self->rehash = hashmap_rehash;
}

int main(void) {
  struct hashmap hs;
  hashmap_init(&hs, 0, 0);

  hs.put(&hs, "foo", 4, 1);
  hs.put(&hs, "bar", 4, 2);
  hs.put(&hs, "baz", 4, 3);

  printf("foo: %d\n", hs.get(&hs, "foo", 4));
  printf("bar: %d\n", hs.get(&hs, "bar", 4));
  printf("baz: %d\n", hs.get(&hs, "baz", 4));
  
  return 0;
}
