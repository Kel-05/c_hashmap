#include <stdio.h>
#include "hashmap.h"

int main(void) {
  hashmap hs;

  hashmap_init(&hs, 0, 0, 0);

  printf("current size: %ld\n", hs.size(&hs));

  int i = 0;
  char c[] = "A hello";

  // put 26 key-value pairs, "A-Z hello" -> 0-25
  for(; i < 26; i++, c[0]++) {
    hs.put(&hs, c, i);
  }

  printf("current size: %ld\n", hs.size(&hs));
  
  // this keys have the same hashcode as "A hello"
  hs.put(&hs, "B gello", 100);
  hs.put(&hs, "C fello", 200);

  printf("current size: %ld\n", hs.size(&hs));

  // get 26 key-value pairs, "A-Z hello" -> 0-25
  for(i = 0, c[0] = 'A'; i < 26; i++, c[0]++) {
    printf("%s: %d\n", c, hs.get(&hs, c));
  }
  
  // get a key-value pair with the same hashcode as "A hello"
  printf("%s: %d\n", "B gello", hs.get(&hs, "B gello"));

  hs.remove(&hs, "B gello"); // remove pair with key "B gello"
  
  // check if "B gello" is removed and if "C fello" is still there
  printf("%s: %d\n", "B gello", hs.get(&hs, "B gello"));
  printf("%s: %d\n", "C fello", hs.get(&hs, "C fello"));

  hs.remove(&hs, "A hello");

  printf("current size: %ld\n", hs.size(&hs));
  
  // check if "C fello" is still there
  printf("%s: %d\n", "A hello", hs.get(&hs, "A hello"));
  printf("%s: %d\n", "C fello", hs.get(&hs, "C fello"));

  hs.remove(&hs, "C fello");
  printf("current size: %ld\n", hs.size(&hs));

  // check if "C fello" is removed correctly
  printf("%s: %d\n", "C fello", hs.get(&hs, "C fello"));

  hs.destroy(&hs); // destroy hashmap
  return 0;
}
