#include <stdio.h>
#include "hashmap.h"

int main(void) {
  hashmap hs;
  hashmap_init(&hs, 0, 0, 0);

  int i = 0;
  char c[] = "A hello";
  
  for(; i < 26; i++, c[0]++) {
    hs.put(&hs, c, i);
  }
  hs.put(&hs, "B gello", 100);
  
  for(i = 0, c[0] = 'A'; i < 26; i++, c[0]++) {
    printf("%s: %d\n", c, hs.get(&hs, c));
  }
  printf("%s: %d\n", "B gello", hs.get(&hs, "B gello"));

  hs.destroy(&hs);
  return 0;
}
