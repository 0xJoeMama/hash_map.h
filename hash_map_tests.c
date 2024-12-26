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

  for (int i = 0; i < 256; i++) {
    if (!hm_put_char_int(&map, i, i)) {
      hm_deinit_char_int(&map);
      return 1;
    }
  }
  for (int i = 0; i < 256; i++) {
    char c = i;
    printf("%c -> %d\n", i, *hm_get_char_int(&map, &c));
  }

  printf("cap = %zu, len = %zu\n", map.cap, map.len);

  hm_deinit_char_int(&map);

  return 0;
}
