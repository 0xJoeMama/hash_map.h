#include "hash_map.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

HM_IMPL(char, int);

uint64_t hash_char(char *c) { return *c; }
int char_eq(char *a, char *b) { return *a == *b; }

int main() {
  HashMap_t(char, int) map;
  // temporary names bc macros take too long to write
  if (!hm_init_char_int(&map, 10, hash_char, char_eq))
    return 1;

  hm_put_char_int(&map, 'a', 42);
  hm_put_char_int(&map, 'b', 42);
  hm_put_char_int(&map, 'a', 43);

  char c = 'a';
  printf("a -> %d\n", *hm_get_char_int(&map, &c));
  c++;
  printf("b -> %d\n", *hm_get_char_int(&map, &c));

  hm_deinit_char_int(&map);

  return 0;
}
