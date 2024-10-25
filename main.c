#include <stdio.h>
#include "hashmap.h"

int main(void) {
  hashmap *hs;

  hs = hashmap_init(0, 0, 0);

  printf("current size: %ld\n", hashmap_size(hs));

  int i = 0;
  char c[] = "A hello";

  // put 26 key-value pairs, "A-Z hello" -> 0-25
  for(; i < 26; i++, c[0]++) {
    hashmap_put(hs, c, i);
  }

  printf("current size: %ld\n", hashmap_size(hs));
  
  // this keys have the same hashcode as "A hello"
  hashmap_put(hs, "B gello", 100);
  hashmap_put(hs, "C fello", 200);

  printf("current size: %ld\n", hashmap_size(hs));

  // get 26 key-value pairs, "A-Z hello" -> 0-25
  for(i = 0, c[0] = 'A'; i < 26; i++, c[0]++) {
    printf("%s: %d\n", c, hashmap_get(hs, c));
  }
  
  // get a key-value pair with the same hashcode as "A hello"
  printf("%s: %d\n", "B gello", hashmap_get(hs, "B gello"));

  hashmap_remove(hs, "B gello"); // remove pair with key "B gello"
  
  // check if "B gello" is removed and if "C fello" is still there
  printf("%s: %d\n", "B gello", hashmap_get(hs, "B gello"));
  printf("%s: %d\n", "C fello", hashmap_get(hs, "C fello"));

  hashmap_remove(hs, "A hello");

  printf("current size: %ld\n", hashmap_size(hs));
  
  // check if "C fello" is still there
  printf("%s: %d\n", "A hello", hashmap_get(hs, "A hello"));
  printf("%s: %d\n", "C fello", hashmap_get(hs, "C fello"));

  hashmap_remove(hs, "C fello");
  printf("current size: %ld\n", hashmap_size(hs));

  // check if "C fello" is removed correctly
  printf("%s: %d\n", "C fello", hashmap_get(hs, "C fello"));

  hashmap_destroy(hs); // destroy hashmap
  return 0;
}
