#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEF_INIT_CAP 16
#define DEF_MAX_CAP (2 << 30)
#define DEF_LOAD_FACTOR 0.75

typedef struct node {
  uint32_t hash;
  char *key;
  int value;
  struct node *next;
} node;

typedef struct hashmap {
  struct node **nodelist;
  uint32_t init_cap;
  uint32_t max_cap;
  uint32_t current_size;
  double load_factor;

  int (*get)(struct hashmap*, char*);
  void (*put)(struct hashmap*, char*, int);
  void (*rehash)(struct hashmap *);
} hashmap;

uint32_t str_hashcode(char *key) {
  uint32_t hash = 0;
  size_t len = strlen(key);
  for(size_t i = 0; i < len; i++) {
    hash += key[i] * 31;
  }
  return hash;
}

void node_destroy(node *self) {
  while(self != NULL) {
    node *tmp = self;
    self = self->next;
    free(tmp->key);
    free(tmp);
  }
}

void hashmap_destroy(hashmap *self) {
  for(uint32_t i = 0; i < self->init_cap; i++) {
    node_destroy(self->nodelist[i]);
  }
  free(self->nodelist);
}

void hashmap_rehash(hashmap *self) {
  uint32_t new_cap = self->init_cap << 1;
  uint32_t old_cap = self->init_cap;
  
  if(new_cap > self->max_cap || new_cap < old_cap) {
    fprintf(stderr, "Error: hashmap_rehash: invalid capacity: %u\n", new_cap);
    abort();
  }

  self->init_cap = new_cap;

  node **new_nodelist = (node **) malloc(sizeof(node *) * new_cap);
  memset(new_nodelist, 0, sizeof(node *) * new_cap);
  node **old_nodelist = self->nodelist;
  self->nodelist = new_nodelist;
  self->current_size = 0;

  for(uint32_t i = 0; i < old_cap; i++) {
    if(old_nodelist[i] != NULL) {
      old_nodelist[i]->hash = str_hashcode(old_nodelist[i]->key) % new_cap;
      self->nodelist[old_nodelist[i]->hash] = old_nodelist[i];
    }
  }

  free(old_nodelist);
}

int hashmap_get(hashmap *self, char *key) {
  uint32_t hash = str_hashcode(key) % self->init_cap;
  
  if(self->init_cap < hash + 1) {
    return -1;
  }

  node *nd = self->nodelist[hash];

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

void hashmap_put(hashmap *self, char *key, int value) {
  uint32_t hash = str_hashcode(key) % self->init_cap;
  node *nd = (node *) malloc(sizeof(node)), *prev_nd;
  node *next_nd = self->nodelist[hash];
  nd->key = (char *) malloc(strlen(key) + 1);
  
  nd->hash = hash;
  strcpy(nd->key, key);
  nd->value = value;
  nd->next = NULL;

  if(next_nd != NULL) {
    prev_nd = next_nd;
    next_nd = next_nd->next;
    while(next_nd != NULL) {
      prev_nd = next_nd;
      next_nd = next_nd->next;
    }
    prev_nd->next = nd;
  }
  else {
    self->nodelist[hash] = nd;
    self->current_size++;
  }

  if((double) self->current_size / self->init_cap >= self->load_factor) {
    self->rehash(self);
  }
  
}

void hashmap_init(hashmap *self, uint32_t init_cap, uint32_t max_cap, double load_factor) {
  self->init_cap = init_cap ? init_cap : DEF_INIT_CAP;
  self->max_cap = max_cap ? max_cap : DEF_MAX_CAP;
  self->load_factor = load_factor ? load_factor : DEF_LOAD_FACTOR;
  
  self->nodelist = (node **) malloc(sizeof(node *) * self->init_cap);
  memset(self->nodelist, 0, sizeof(node *) * self->init_cap);
  
  self->get = hashmap_get;
  self->put = hashmap_put;
  self->rehash = hashmap_rehash;
}

int main(void) {
  struct hashmap hs;
  hashmap_init(&hs, 0, 0, 0);

  int i = 0;
  char c[] = "A hello";

  for(; i < 26; i++, c[0]++) {
    hs.put(&hs, c, i);
  }
  
  for(i = 0, c[0] = 'A'; i < 26; i++, c[0]++) {
    printf("%s: %d\n", c, hs.get(&hs, c));
  }

  hashmap_destroy(&hs);
  return 0;
}
