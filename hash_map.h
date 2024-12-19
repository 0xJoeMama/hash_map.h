#ifndef HM_H
#include <inttypes.h>
#include <stddef.h>

#include "dynamic_array.h/dynamic_array_generic.h"

#define HashMap(key, value) HashMap_##key##_##value
#define HashMap_t(key, value) HashMap_##key##_##value_t
#define KVPair_t(key, value) KVPair_##key##_##value##_t
#define HashFunc_t(key) Hash_##key
#define EqFunc_t(key) Eq_##key
#define hm_function(ret_type, name, key, value, ...)                           \
  ret_type name##_##key##_##value(__VA_ARGS__)

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
  hm_function(int, hm_init, key, value, struct HashMap(key, value) * hm,       \
              size_t initial_cap, HashFunc_t(key) hash, EqFunc_t(key) eq);     \
  hm_function(int, hm_put, key, value, struct HashMap(key, value) * hm, key k, \
              value v);                                                        \
  hm_function(value *, hm_get, key, value, struct HashMap(key, value) * hm,    \
              key * k);                                                        \
  hm_function(void, hm_deinit, key, value, struct HashMap(key, value) * hm);   \
  typedef struct HashMap(key, value) HashMap_t(key, value)

#define HM_IMPL(key, value)                                                    \
  typedef struct {                                                             \
    key k;                                                                     \
    value v;                                                                   \
    uint64_t hash;                                                             \
  } KVPair_t(key, value);                                                      \
                                                                               \
  DA_IMPL(KVPair_t(key, value));                                               \
                                                                               \
  typedef uint64_t (*HashFunc_t(key))(key *);                                  \
  typedef int (*EqFunc_t(key))(key *, key *);                                  \
                                                                               \
  struct HashMap(key, value) {                                                 \
    DynamicArray_t(KVPair_t(key, value)) buckets;                              \
    HashFunc_t(key) hash;                                                      \
    EqFunc_t(key) eq;                                                          \
  };                                                                           \
                                                                               \
  hm_function(int, hm_init, key, value, struct HashMap(key, value) * hm,       \
              size_t initial_cap, HashFunc_t(key) hash, EqFunc_t(key) eq) {    \
    if (!da_init(&hm->buckets, initial_cap, KVPair_t(key, value)))             \
      return 0;                                                                \
    hm->hash = hash;                                                           \
    hm->eq = eq;                                                               \
    return 1;                                                                  \
  }                                                                            \
                                                                               \
  hm_function(void, hm_deinit, key, value, struct HashMap(key, value) * hm) {  \
    /* delegate to the bucket array */                                         \
    da_deinit(&hm->buckets, KVPair_t(key, value));                             \
  }                                                                            \
                                                                               \
  typedef struct HashMap(key, value) HashMap_t(key, value)

#define HM_H
#endif
