#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "hashfuncs.h"
#include "hashset.h"


/* Wrapper to match HashFunction signature for strings */
static size_t
str_hash(const void *data)
{
    return hash_fnv1a_str((const char *)data);
}

/* Wrapper for fixed-size int hashing */
static size_t
int_hash(const void *data)
{
    return hash_murmur3(data, sizeof(int), 0xdeadbeef);
}


static void
test_basic(void)
{
    printf("--- test_basic ---\n");

    HashSet set = HashSet_new(sizeof(int), int_hash);
    assert(set);

    int a = 10, b = 20, c = 30, d = 99;

    HashSet_add(set, &a);
    HashSet_add(set, &b);
    HashSet_add(set, &c);

    assert( HashSet_has(set, &a));
    assert( HashSet_has(set, &b));
    assert( HashSet_has(set, &c));
    assert(!HashSet_has(set, &d));

    printf("  add/has: PASS\n");

    HashSet_remove(set, &b);
    assert( HashSet_has(set, &a));
    assert(!HashSet_has(set, &b));
    assert( HashSet_has(set, &c));

    printf("  remove: PASS\n");

    /* Adding duplicate should not change anything */
    HashSet_add(set, &a);
    assert(HashSet_has(set, &a));

    printf("  duplicate add: PASS\n");

    HashSet_free(set);
    printf("  free: PASS\n");
}


static void
test_strings(void)
{
    printf("--- test_strings ---\n");

    /* Store fixed-size char buffers for simplicity */
    #define STR_SIZE 32
    HashSet set = HashSet_new(STR_SIZE, str_hash);
    assert(set);

    char foo[STR_SIZE]  = "foo";
    char bar[STR_SIZE]  = "bar";
    char baz[STR_SIZE]  = "baz";
    char quux[STR_SIZE] = "quux";

    HashSet_add(set, foo);
    HashSet_add(set, bar);
    HashSet_add(set, baz);

    assert( HashSet_has(set, foo));
    assert( HashSet_has(set, bar));
    assert( HashSet_has(set, baz));
    assert(!HashSet_has(set, quux));

    printf("  string add/has: PASS\n");

    HashSet_remove(set, bar);
    assert( HashSet_has(set, foo));
    assert(!HashSet_has(set, bar));
    assert( HashSet_has(set, baz));

    printf("  string remove: PASS\n");

    HashSet_free(set);
    printf("  free: PASS\n");
    #undef STR_SIZE
}


static void
test_collisions(void)
{
    printf("--- test_collisions ---\n");

    /* Force collisions by using a tiny initial capacity.
       HashSet_new takes capacity 2 so almost everything collides. */
    HashSet set = HashSet_new(sizeof(int), int_hash);
    assert(set);

    /* Add enough entries to force chaining */
    int vals[16];
    for (int i = 0; i < 16; i++) {
        vals[i] = i * 100;
        HashSet_add(set, &vals[i]);
    }

    /* All should be found */
    for (int i = 0; i < 16; i++)
        assert(HashSet_has(set, &vals[i]));

    printf("  collision add/has: PASS\n");

    /* Remove every other one */
    for (int i = 0; i < 16; i += 2)
        HashSet_remove(set, &vals[i]);

    for (int i = 0; i < 16; i++) {
        if (i % 2 == 0)
            assert(!HashSet_has(set, &vals[i]));
        else
            assert( HashSet_has(set, &vals[i]));
    }

    printf("  collision remove: PASS\n");

    HashSet_free(set);
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
