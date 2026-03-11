#include "hashfuncs.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>


/*
    FNV-1a (32/64-bit auto-selected based on size_t width)
    Public domain.
*/
size_t
hash_fnv1a(const void *data, size_t size)
{
#if SIZE_MAX == 0xFFFFFFFF
    uint32_t hash = FNV_OFFSET_BASIS_32;
    const unsigned char *p = (const unsigned char *)data;
    for (size_t i = 0; i < size; i++) {
        hash ^= p[i];
        hash *= FNV_PRIME_32;
    }
    return (size_t)hash;
#else
    uint64_t hash = FNV_OFFSET_BASIS_64;
    const unsigned char *p = (const unsigned char *)data;
    for (size_t i = 0; i < size; i++) {
        hash ^= p[i];
        hash *= FNV_PRIME_64;
    }
    return (size_t)hash;
#endif
}

size_t
hash_fnv1a_str(const char *str)
{
    return hash_fnv1a(str, strlen(str));
}


/*
    MurmurHash3 (32-bit finalizer mixed up to size_t)
    Public domain — Austin Appleby.
*/
size_t
hash_murmur3(const void *data, size_t size, uint32_t seed)
{
    const uint8_t *p     = (const uint8_t *)data;
    const size_t   nblocks = size / 4;
    uint32_t       h1    = seed;

    const uint32_t c1 = 0xcc9e2d51u;
    const uint32_t c2 = 0x1b873593u;

    /* Body — process 4-byte blocks */
    const uint32_t *blocks = (const uint32_t *)p;
    for (size_t i = 0; i < nblocks; i++) {
        uint32_t k1;
        memcpy(&k1, &blocks[i], sizeof(k1));  /* safe unaligned read */

        k1 *= c1;
        k1  = (k1 << 15) | (k1 >> 17);       /* rotl32 */
        k1 *= c2;

        h1 ^= k1;
        h1  = (h1 << 13) | (h1 >> 19);       /* rotl32 */
        h1  = h1 * 5 + 0xe6546b64u;
    }

    /* Tail — remaining bytes */
    const uint8_t *tail = p + nblocks * 4;
    uint32_t k1 = 0;
    switch (size & 3) {
        case 3: k1 ^= (uint32_t)tail[2] << 16; /* fall through */
        case 2: k1 ^= (uint32_t)tail[1] << 8;  /* fall through */
        case 1: k1 ^= (uint32_t)tail[0];
                k1 *= c1;
                k1  = (k1 << 15) | (k1 >> 17);
                k1 *= c2;
                h1 ^= k1;
    }

    /* Finalization mix */
    h1 ^= (uint32_t)size;
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6bu;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35u;
    h1 ^= h1 >> 16;

    return (size_t)h1;
}
