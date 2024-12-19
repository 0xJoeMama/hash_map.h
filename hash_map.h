#ifndef HM_H
#include <inttypes.h>
#include <stddef.h>

#include "dynamic_array.h/dynamic_array.h"

#define HashMap(key, value) HashMap_##key##_##value
#define HashMap_t(key, value) HashMap_t_##key##_##value
#define KVPair_t(key, value) KVPair_##key##_##value
#define HashFunc_t(key) Hash_##key
#define EqFunc_t(key) Eq_##key

#define HM_DECLARE(key, value)                                                 \
  typedef struct {                                                             \
    key k;                                                                     \
    value v;                                                                   \
    uint64_t hash;                                                             \
  } KVPair_t(key, value);                                                      \
                                                                               \
  DA_DECLARE(KVPair_t(key, value));                                            \
                                                                               \
  typedef uint64_t (*HashFunc_t(key))(key *);                                  \
  typedef int (*EqFunc_t(key))(key *, key *);                                  \
                                                                               \
  struct HashMap(key, value) {                                                 \
    DynamicArray_t(KVPair_t(key, value)) * buckets;                            \
    HashFunc_t(key) hash;                                                      \
    EqFunc_t(key) eq;                                                          \
  };                                                                           \
                                                                               \
  int hm_put(struct HashMap(key, value) * hm, key k, value v);                 \
  value *hm_get(struct HashMap(key, value) * hm, key * k);                     \
  typedef struct HashMap(key, value) HashMap_t(key, value)

#define HM_H
#endif
