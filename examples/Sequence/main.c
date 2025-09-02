#include <stdio.h>

#include "sequence.h"



#define PRINT_SEQ(seq, name) do { \
    printf("%s: Length=%zu, Capacity=%zu, Contents=[", \
           name, Sequence_length(seq), Sequence_capacity(seq)); \
    for (size_t i = 0; i < Sequence_length(seq); i++) { \
        printf("%d", *(int*)Sequence_at(seq, i)); \
        if (i < Sequence_length(seq) - 1) printf(", "); \
    } \
    printf("]\n"); \
} while(0)

#define SEPARATOR() printf("\n==============================================\n")

int main(void) {
    printf("BTCHWRK Sequence API Complete Test\n");
    SEPARATOR();
    
    // Test 1: Constructor
    printf("1. Testing Sequence_new()\n");
    Sequence seq = Sequence_new(sizeof(int), 4);
    PRINT_SEQ(seq, "New sequence");
    
    // Test 2: Adding single elements
    printf("\n2. Testing Sequence_add()\n");
    int vals[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        Sequence_add(seq, &vals[i]);
        printf("Added %d: ", vals[i]);
        PRINT_SEQ(seq, "");
    }
    
    // Test 3: Appending multiple elements
    printf("\n3. Testing Sequence_append()\n");
    int more_vals[] = {40, 50, 60, 70};
    Sequence_append(seq, more_vals, 4);
    PRINT_SEQ(seq, "After appending [40,50,60,70]");
    
    // Test 4: Testing capacity and growth
    printf("\n4. Testing automatic growth\n");
    printf("Original capacity was 4, now it's %zu (auto-grew)\n", Sequence_capacity(seq));
    
    // Test 5: Manual growth
    printf("\n5. Testing Sequence_grow()\n");
    Sequence_grow(seq);
    PRINT_SEQ(seq, "After manual grow");
    
    // Test 6: Access elements
    printf("\n6. Testing Sequence_at()\n");
    printf("Element at index 0: %d\n", *(int*)Sequence_at(seq, 0));
    printf("Element at index 3: %d\n", *(int*)Sequence_at(seq, 3));
    printf("Element at index 6: %d\n", *(int*)Sequence_at(seq, 6));
    
    // Test 7: Insert elements
    printf("\n7. Testing Sequence_insert()\n");
    int insert_vals[] = {15, 25};
    Sequence_insert(seq, 1, insert_vals, 2);  // Insert at index 1
    PRINT_SEQ(seq, "After inserting [15,25] at index 1");
    
    // Test 8: Replace elements
    printf("\n8. Testing Sequence_replace()\n");
    int replace_vals[] = {99, 88};
    Sequence_replace(seq, 2, replace_vals, 2);  // Replace 2 elements starting at index 2
    PRINT_SEQ(seq, "After replacing 2 elements at index 2 with [99,88]");
    
    // Test 9: Delete elements
    printf("\n9. Testing Sequence_delete()\n");
    Sequence_delete(seq, 3, 2);  // Delete 2 elements starting at index 3
    PRINT_SEQ(seq, "After deleting 2 elements starting at index 3");
    
    // Test 10: Create second sequence for concatenation
    printf("\n10. Testing Sequence_concat()\n");
    Sequence seq2 = Sequence_new(sizeof(int), 3);
    int seq2_vals[] = {100, 200, 300};
    Sequence_append(seq2, seq2_vals, 3);
    PRINT_SEQ(seq2, "Second sequence");
    
    Sequence_concat(seq, seq2);
    PRINT_SEQ(seq, "After concatenating seq2 to seq");
    
    // Test 11: Shrink to fit
    printf("\n11. Testing Sequence_shrink()\n");
    printf("Before shrink - Capacity: %zu\n", Sequence_capacity(seq));
    Sequence_shrink(seq);
    PRINT_SEQ(seq, "After shrinking");
    
    // Test 12: Length and capacity functions
    printf("\n12. Testing utility functions\n");
    printf("Sequence_length(seq): %zu\n", Sequence_length(seq));
    printf("Sequence_capacity(seq): %zu\n", Sequence_capacity(seq));
    
    // Test 13: Edge cases
    printf("\n13. Testing edge cases\n");
    
    // Empty sequence operations
    Sequence empty_seq = Sequence_new(sizeof(int), 2);
    PRINT_SEQ(empty_seq, "Empty sequence");
    
    // Add to empty
    int edge_val = 42;
    Sequence_add(empty_seq, &edge_val);
    PRINT_SEQ(empty_seq, "After adding to empty");
    
    // Insert at beginning
    int begin_val = 1;
    Sequence_insert(empty_seq, 0, &begin_val, 1);
    PRINT_SEQ(empty_seq, "After inserting at beginning");
    
    // Insert at end
    int end_val = 99;
    Sequence_insert(empty_seq, Sequence_length(empty_seq), &end_val, 1);
    PRINT_SEQ(empty_seq, "After inserting at end");
    
    // Test 14: Large append to test multiple growth cycles
    printf("\n14. Testing multiple growth cycles\n");
    Sequence growth_seq = Sequence_new(sizeof(int), 1);  // Start very small
    int large_array[20];
    for (int i = 0; i < 20; i++) {
        large_array[i] = i * 10;
    }
    Sequence_append(growth_seq, large_array, 20);
    printf("Added 20 elements to sequence with initial capacity 1\n");
    PRINT_SEQ(growth_seq, "Growth test result");
    
    // Cleanup
    printf("\n15. Cleanup\n");
    Sequence_free(seq);
    Sequence_free(seq2);
    Sequence_free(empty_seq);
    Sequence_free(growth_seq);
    printf("All sequences freed successfully\n");
    
    SEPARATOR();
    printf("✅ All tests completed!\n");
    
    return 0;
}
