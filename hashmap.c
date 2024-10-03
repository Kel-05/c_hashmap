#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

#define DEF_INIT_CAP 16
#define DEF_MAX_CAP (2 << 30)
#define DEF_LOAD_FACTOR 0.75

typedef struct node {
  uint32_t hash;
  char *key;
  int value;
  struct node *next;
} node;

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

  node **old_nodelist = self->nodelist;
  self->nodelist = (node **) malloc(sizeof(node *) * new_cap);
  memset(self->nodelist, 0, sizeof(node *) * new_cap);
  self->current_size = 0;

  for(uint32_t i = 0; i < old_cap; i++) {
    if(old_nodelist[i] == NULL) continue; 
    old_nodelist[i]->hash = str_hashcode(old_nodelist[i]->key) % new_cap;
    self->nodelist[old_nodelist[i]->hash] = old_nodelist[i];
  }
  
  free(old_nodelist);
}

int hashmap_get(hashmap *self, char *key) {
  uint32_t hash = str_hashcode(key) % self->init_cap;
  node *nd = self->nodelist[hash];

  if(nd == NULL) {
    return -1;
  }
  if(nd->next == NULL) {
    return nd->value;
  }
  while(nd != NULL) {
    if(strcmp(nd->key, key) == 0) {
      return nd->value;
    }
    nd = nd->next;
  }
  
  return -1;
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
  while(*next_nd != NULL) {
    next_nd = &(*next_nd)->next;
  }
  *next_nd = nd;

  if((double) self->current_size / self->init_cap >= self->load_factor) {
    hashmap_rehash(self);
  }
}

void hashmap_init(hashmap *self, uint32_t init_cap, uint32_t max_cap, double load_factor) {
  self->init_cap = init_cap ? init_cap : DEF_INIT_CAP;
  self->max_cap = max_cap ? max_cap : DEF_MAX_CAP;
  self->load_factor = load_factor <= 1 && load_factor > 0 ? load_factor : DEF_LOAD_FACTOR;
  
  self->nodelist = (node **) malloc(sizeof(node *) * self->init_cap);
  memset(self->nodelist, 0, sizeof(node *) * self->init_cap);
  
  self->get = hashmap_get;
  self->put = hashmap_put;
  self->destroy = hashmap_destroy;
}
