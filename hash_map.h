#ifndef HM_H
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>

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
    int occupied;                                                              \
  } KVPair_t(key, value);                                                      \
                                                                               \
  typedef uint64_t (*HashFunc_t(key))(key *);                                  \
  typedef int (*EqFunc_t(key))(key *, key *);                                  \
                                                                               \
  struct HashMap(key, value) {                                                 \
    KVPair_t(key, value) * buckets;                                            \
    HashFunc_t(key) hash;                                                      \
    EqFunc_t(key) eq;                                                          \
    size_t cap;                                                                \
    size_t len;                                                                \
  };                                                                           \
                                                                               \
  hm_function(int, hm_init, key, value, struct HashMap(key, value) * hm,       \
              size_t initial_cap, HashFunc_t(key) hash, EqFunc_t(key) eq) {    \
    hm->buckets = (KVPair_t(key, value) *)calloc(                              \
        initial_cap, sizeof(KVPair_t(key, value)));                            \
                                                                               \
    if (!hm->buckets)                                                          \
      return 0;                                                                \
                                                                               \
    hm->cap = initial_cap;                                                     \
    hm->len = 0;                                                               \
    hm->hash = hash;                                                           \
    hm->eq = eq;                                                               \
    return 1;                                                                  \
  }                                                                            \
                                                                               \
  hm_function(int, hm_put, key, value, struct HashMap(key, value) * hm, key k, \
              value v) {                                                       \
    uint64_t hash = hm->hash(&k);                                              \
    uint64_t reduced_hash = hash % hm->cap;                                    \
    KVPair_t(key, value) *target_entry = hm->buckets + reduced_hash;           \
    /* a free bucket allows us to just insert the data into the array without  \
     * any problems */                                                         \
    if (!target_entry->occupied) {                                             \
      target_entry->k = k;                                                     \
      target_entry->v = v;                                                     \
      target_entry->hash = hash;                                               \
      target_entry->occupied = 1;                                              \
      hm->len++;                                                               \
      return 0;                                                                \
    }                                                                          \
                                                                               \
    /* if the key is equal to the old key and the hashes match, just replace   \
     * the value in that bucket */                                             \
    if (hm->eq(&k, &target_entry->k)) {                                        \
      target_entry->v = v;                                                     \
      /* hm len remains constant */                                            \
      return 0;                                                                \
    }                                                                          \
                                                                               \
    /* TODO: collision */                                                      \
    return 1;                                                                  \
  }                                                                            \
                                                                               \
  hm_function(value *, hm_get, key, value, struct HashMap(key, value) * hm,    \
              key * k) {                                                       \
    uint64_t hash = hm->hash(k);                                               \
    uint64_t reduced_hash = hash % hm->cap;                                    \
    KVPair_t(key, value) *target_entry = hm->buckets + reduced_hash;           \
                                                                               \
    if (!target_entry->occupied || !hm->eq(&target_entry->k, k))               \
      return NULL;                                                             \
                                                                               \
    return &target_entry->v;                                                   \
  }                                                                            \
                                                                               \
  hm_function(void, hm_deinit, key, value, struct HashMap(key, value) * hm) {  \
    free(hm->buckets);                                                         \
    hm->len = 0;                                                               \
    hm->cap = 0;                                                               \
  }                                                                            \
                                                                               \
  typedef struct HashMap(key, value) HashMap_t(key, value)

#define HM_H
#endif
