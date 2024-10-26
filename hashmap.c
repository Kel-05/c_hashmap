#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

#define DEF_INIT_CAP 16
#define DEF_MAX_CAP (1 << 30)
#define DEF_LOAD_FACTOR 0.75f


typedef struct node {
  uint32_t hash;
  char *key;
  int value;
  struct node *next;
} node;

typedef struct hashmap {
  node **nodelist;
  uint32_t init_cap;
  uint32_t max_cap;
  uint32_t current_size;
  float load_factor;
} hashmap;


uint32_t str_hashcode(char *key) {
  uint32_t hash = 0;
  size_t len = strlen(key);
  
  for(size_t i = 0; i < len; i++) {
    hash += key[i] * 31;
  }
  
  return hash;
}


static void node_destroy(node *self) {
  while(self != NULL) {
    node *tmp = self;
    self = self->next;
    free(tmp->key);
    free(tmp);
  }
}


static node *node_get(hashmap *self, char *key, int prevflag) {
  uint32_t hash = str_hashcode(key) % self->init_cap;
  node *nd = self->nodelist[hash], *prev_nd = nd;
  
  while(nd != NULL) {
    if(strcmp(nd->key, key) == 0) {
      if(prevflag) return prev_nd;
      return nd;
    }
    
    prev_nd = nd;
    nd = nd->next;
  }
  
  return NULL;
}


void hashmap_destroy(hashmap *self) {
  for(uint32_t i = 0; i < self->init_cap; i++) {
    node_destroy(self->nodelist[i]);
  }
  
  free(self->nodelist);
  free(self);
}


size_t hashmap_size(hashmap *self) {
  return self->current_size;
}


int hashmap_get(hashmap *self, char *key) {
  node *nd = node_get(self, key, 0);

  if(nd != NULL) {
    return nd->value;
  }
  
  return -1;
}


static void hashmap_rehash(hashmap *self) {
  uint32_t new_cap = self->init_cap << 1;
  uint32_t old_cap = self->init_cap;
  
  if(new_cap > self->max_cap || new_cap < old_cap) {
    fprintf(stderr, "hashmap_rehash: invalid capacity: %u\n", new_cap);
    abort();
  }
  
  self->init_cap = new_cap;
  node **old_nodelist = self->nodelist;
  self->nodelist = (node **) malloc(sizeof(node *) * new_cap);
  
  memset(self->nodelist, 0, sizeof(node *) * new_cap);
  self->current_size = 0;

  for(uint32_t i = 0; i < old_cap; i++) {
    if(old_nodelist[i] == NULL) continue;
    node *nd, *old_nd;
    nd = old_nodelist[i];

    while(nd != NULL) {
      hashmap_put(self, nd->key, nd->value);
      old_nd = nd;
      nd = nd->next;
      free(old_nd->key);
      free(old_nd);
    }
  }
  
  free(old_nodelist);
}


void hashmap_put(hashmap *self, char *key, int value) {
  uint32_t hash = str_hashcode(key) % self->init_cap;
  node *nd = (node *) malloc(sizeof(node));
  node **next_nd = &self->nodelist[hash];

  nd->key = (char *) malloc(strlen(key) + 1);
  memset(nd->key, 0, strlen(key) + 1);
  
  nd->hash = hash;
  strcpy(nd->key, key);
  nd->value = value;
  nd->next = NULL;

  if(*next_nd == NULL) {
    self->current_size++;
  }
  else {
    next_nd = &(*next_nd)->next;
    while(*next_nd != NULL) {
      next_nd = &(*next_nd)->next;
    }
  }
  
  *next_nd = nd;

  if((float) self->current_size / self->init_cap >= self->load_factor) {
    hashmap_rehash(self);
  }
}


void hashmap_remove(hashmap *self, char *key) {
  node *prev_nd = node_get(self, key, 1);
  node *nd = node_get(self, key, 0);

  if(prev_nd == NULL || nd == NULL) return;

  if(prev_nd == nd) {
    self->nodelist[nd->hash] = nd->next;
    if(nd->next == NULL) self->current_size--;
    free(nd->key);
    free(nd);
    return;
  }
  
  prev_nd->next = nd->next;
  free(nd->key);
  free(nd);
}


hashmap *hashmap_init(uint32_t init_cap, uint32_t max_cap, float load_factor) {
  hashmap *self = (hashmap *) malloc(sizeof(hashmap));
  
  self->init_cap = init_cap ? init_cap : DEF_INIT_CAP;
  self->max_cap = max_cap ? max_cap : DEF_MAX_CAP;
  self->load_factor =
    load_factor <= 1 && load_factor > 0 ? load_factor : DEF_LOAD_FACTOR;

  self->current_size = 0;

  self->nodelist = (node **) malloc(sizeof(node *) * self->init_cap);
  memset(self->nodelist, 0, sizeof(node *) * self->init_cap);

  return self;
}
