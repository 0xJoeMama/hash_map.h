#include "hash_map.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

HM_IMPL(char, int);

uint64_t hash_char(char *c) { return *c; }
int char_eq(char *a, char *b) { return *a == *b; }

int main() {
  HashMap_t(char, int) map;
  // temporary names bc macros take too long to write
  if (!hm_init(&map, 10, hash_char, char_eq, char, int))
    return 1;

  for (int i = 0; i < 256; i++) {
    if (!hm_put(&map, i, i, char, int)) {
      hm_deinit(&map, char, int);
      return 1;
    }
  }
  for (int i = 0; i < 256; i++) {
    char c = i;
    printf("%c -> %d\n", i, *hm_get_char_int(&map, &c));
    int c_p;
    if (!hm_remove(&map, &c, &c_p, char, int)) {
      hm_deinit(&map, char, int);
      return 1;
    }

    assert(hm_get(&map, &c, char, int) == NULL && "woopsie");
  }

  printf("cap = %zu, len = %zu\n", map.cap, map.len);
  hm_put(&map, 'a', 32, char, int);
  char c = 'a';
  *hm_get(&map, &c, char, int) += 10;
  printf("%c -> %d\n", c, *hm_get(&map, &c, char, int));

  hm_deinit(&map, char, int);

  return 0;
}
