#ifndef HASHMAP_H_
#define HASHMAP_H_
#include <stdint.h>

struct node;

typedef struct hashmap {
  struct node **nodelist;
  uint32_t init_cap;
  uint32_t max_cap;
  uint32_t current_size;
  double load_factor;

  int (*get)(struct hashmap*, char*);
  void (*put)(struct hashmap*, char*, int);
  void (*destroy)(struct hashmap *);
} hashmap;

void hashmap_init(struct hashmap *self, uint32_t init_cap, uint32_t max_cap, double load_factor);

#endif
