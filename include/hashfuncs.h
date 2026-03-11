#ifndef BTCHWRK_HASH_H
#define BTCHWRK_HASH_H


#include <stddef.h>
#include <stdint.h>


/* FNV-1a constants */
#define FNV_OFFSET_BASIS_32 2166136261u
#define FNV_PRIME_32        16777619u
#define FNV_OFFSET_BASIS_64 14695981039346656037ull
#define FNV_PRIME_64        1099511628211ull


/*
    FNV-1a — public domain
    Fast, simple, good distribution for short strings.
    Ideal for string interning.
*/
size_t hash_fnv1a(const void *data, size_t size);
size_t hash_fnv1a_str(const char *str);

/*
    MurmurHash3 — public domain (Austin Appleby)
    Better avalanche effect for longer or arbitrary data.
    Ideal for general purpose hashing.
*/
size_t hash_murmur3(const void *data, size_t size, uint32_t seed);


#endif /* BTCHWRK_HASH_H */
