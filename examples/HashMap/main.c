#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "hashfuncs.h"
#include "hashmap.h"


/*
    Hash / compare functions
*/
static size_t
int_hash(const void *data, size_t size)
{
    return hash_murmur3(data, size, 0xdeadbeef);
}

static size_t
str_hash(const void *data, size_t size)
{
    (void)size;
    return hash_fnv1a_str((const char *)data);
}

static int
str_cmp(const void *a, const void *b, size_t size)
{
    (void)size;
    return strcmp((const char *)a, (const char *)b);
}


static void
test_basic(void)
{
    printf("--- test_basic ---\n");

    HashMap map = HashMap_new(sizeof(int), sizeof(int), int_hash, NULL);
    assert(map);

    int k1 = 1, v1 = 100;
    int k2 = 2, v2 = 200;
    int k3 = 3, v3 = 300;
    int missing = 99;

    HashMap_put(map, &k1, &v1);
    HashMap_put(map, &k2, &v2);
    HashMap_put(map, &k3, &v3);

    assert( HashMap_has(map, &k1));
    assert( HashMap_has(map, &k2));
    assert( HashMap_has(map, &k3));
    assert(!HashMap_has(map, &missing));
    printf("  put/has: PASS\n");

    assert(*(int *)HashMap_get(map, &k1) == 100);
    assert(*(int *)HashMap_get(map, &k2) == 200);
    assert(*(int *)HashMap_get(map, &k3) == 300);
    assert(HashMap_get(map, &missing) == NULL);
    printf("  get: PASS\n");

    /* Update an existing key — value should change, entry count should not */
    int v1_updated = 999;
    HashMap_put(map, &k1, &v1_updated);
    assert(*(int *)HashMap_get(map, &k1) == 999);
    printf("  update: PASS\n");

    HashMap_remove(map, &k2);
    assert( HashMap_has(map, &k1));
    assert(!HashMap_has(map, &k2));
    assert( HashMap_has(map, &k3));
    printf("  remove: PASS\n");

    HashMap_free(map);
    printf("  free: PASS\n");
}


static void
test_strings(void)
{
    printf("--- test_strings ---\n");

#define KEY_SIZE 32
    HashMap map = HashMap_new(KEY_SIZE, sizeof(int), str_hash, str_cmp);
    assert(map);

    char k_foo[KEY_SIZE]  = "foo";
    char k_bar[KEY_SIZE]  = "bar";
    char k_baz[KEY_SIZE]  = "baz";
    char k_quux[KEY_SIZE] = "quux";
    int  v_foo = 1, v_bar = 2, v_baz = 3;

    HashMap_put(map, k_foo, &v_foo);
    HashMap_put(map, k_bar, &v_bar);
    HashMap_put(map, k_baz, &v_baz);

    assert(*(int *)HashMap_get(map, k_foo) == 1);
    assert(*(int *)HashMap_get(map, k_bar) == 2);
    assert(*(int *)HashMap_get(map, k_baz) == 3);
    assert(HashMap_get(map, k_quux) == NULL);
    printf("  put/get: PASS\n");

    HashMap_remove(map, k_bar);
    assert( HashMap_has(map, k_foo));
    assert(!HashMap_has(map, k_bar));
    assert( HashMap_has(map, k_baz));
    printf("  remove: PASS\n");

    HashMap_free(map);
    printf("  free: PASS\n");
#undef KEY_SIZE
}


static void
test_collisions(void)
{
    printf("--- test_collisions ---\n");

    HashMap map = HashMap_new(sizeof(int), sizeof(int), int_hash, NULL);
    assert(map);

    int vals[16];
    for (int i = 0; i < 16; i++) {
        vals[i] = i * 100;
        HashMap_put(map, &vals[i], &i);
    }

    for (int i = 0; i < 16; i++)
        assert(*(int *)HashMap_get(map, &vals[i]) == i);
    printf("  collision put/get: PASS\n");

    for (int i = 0; i < 16; i += 2)
        HashMap_remove(map, &vals[i]);

    for (int i = 0; i < 16; i++) {
        if (i % 2 == 0) assert( HashMap_get(map, &vals[i]) == NULL);
        else            assert(*(int *)HashMap_get(map, &vals[i]) == i);
    }
    printf("  collision remove: PASS\n");

    HashMap_free(map);
    printf("  free: PASS\n");
}


int
main(void)
{
    test_basic();
    test_strings();
    test_collisions();

    printf("\nAll tests passed.\n");
    return 0;
}
