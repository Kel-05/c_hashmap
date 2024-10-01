#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_INITIAL_CAPACITY 16
#define DEFAULT_MAXIMUM_CAPACITY (2 << 30)
#define DEFAULT_LOAD_FACTOR 0.75

typedef struct node {
  int hash;
  char *key;
  int value;
  struct node *next;
} node;

typedef struct hashmap {
  struct node *table;
  int initial_capacity;
  int maximum_capacity;
  double load_factor;

  void (*init)(struct hashmap *self, int initial_capacity, int maximum_capacity, double load_factor);
  int (*get)(struct hashmap *self);
  void (*put)(struct hashmap *self, char *key);
} hashmap;

void put(hashmap *self, char *key) {
  
}

void init(hashmap *self, int initial_capacity, int maximum_capacity, double load_factor) {
  self->initial_capacity = initial_capacity ? initial_capacity : DEFAULT_INITIAL_CAPACITY;
  self->initial_capacity = maximum_capacity ? maximum_capacity : DEFAULT_MAXIMUM_CAPACITY;
  self->load_factor = load_factor ? load_factor : DEFAULT_LOAD_FACTOR;
  self->table = (struct node*)malloc(sizeof(struct node) * self->initial_capacity);
}

int main(void) {
  struct hashmap hs;
  hs.init = init;
  
  return 0;
}
