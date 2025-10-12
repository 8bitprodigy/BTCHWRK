#include <stdio.h>
#include <assert.h>
#include "dynamicarray.h"

int main(void)
{
    printf("=== Dynamic Array Test ===\n\n");
    
    // Test 1: Create and add integers
    printf("Test 1: Creating array and adding elements\n");
    void *int_array = DynamicArray_new(sizeof(int), 2);
    
    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        DynamicArray_add(int_array, &values[i]);  // Pass address of pointer
        printf("Added %d, length: %zu, capacity: %zu\n", 
               values[i], 
               DynamicArray_length(int_array),
               DynamicArray_capacity(int_array));
    }
    
    // Test 2: Access elements using array notation
    printf("\nTest 2: Accessing elements\n");
    int *arr = (int*)int_array;
    for (size_t i = 0; i < DynamicArray_length(int_array); i++) {
        printf("array[%zu] = %d\n", i, arr[i]);
    }
    
    // Test 3: Insert elements
    printf("\nTest 3: Inserting element at index 2\n");
    int insert_val = 25;
    DynamicArray_insert(&int_array, 2, &insert_val, 1);  // Pass address
    arr = (int*)int_array;  // Refresh pointer
    for (size_t i = 0; i < DynamicArray_length(int_array); i++) {
        printf("array[%zu] = %d\n", i, arr[i]);
    }
    
    // Test 4: Delete elements
    printf("\nTest 4: Deleting 2 elements at index 1\n");
    DynamicArray_delete(int_array, 1, 2);  // No & needed (doesn't reallocate)
    for (size_t i = 0; i < DynamicArray_length(int_array); i++) {
        printf("array[%zu] = %d\n", i, arr[i]);
    }
    
    // Test 5: Replace elements
    printf("\nTest 5: Replacing element at index 1\n");
    int replace_val = 99;
    DynamicArray_replace(&int_array, 1, &replace_val, 1);  // Pass address
    arr = (int*)int_array;  // Refresh pointer
    for (size_t i = 0; i < DynamicArray_length(int_array); i++) {
        printf("array[%zu] = %d\n", i, arr[i]);
    }
    
    // Test 6: Concatenation
    printf("\nTest 6: Concatenating two arrays\n");
    void *int_array2 = DynamicArray_new(sizeof(int), 2);
    int more_values[] = {100, 200};
    DynamicArray_append(&int_array2, more_values, 2);  // Pass address
    
    DynamicArray_concat(&int_array, int_array2);  // First arg is address
    printf("After concat, length: %zu\n", DynamicArray_length(int_array));
    arr = (int*)int_array;  // Refresh pointer
    for (size_t i = 0; i < DynamicArray_length(int_array); i++) {
        printf("array[%zu] = %d\n", i, arr[i]);
    }
    
    // Test 7: Shrink
    printf("\nTest 7: Shrinking array\n");
    printf("Before shrink - capacity: %zu\n", DynamicArray_capacity(int_array));
    DynamicArray_shrink(int_array);  // No & needed (explicit operation)
    printf("After shrink - capacity: %zu\n", DynamicArray_capacity(int_array));
    
    // Cleanup
    DynamicArray_free(int_array);
    DynamicArray_free(int_array2);
    
    printf("\nAll tests completed successfully!\n");
    return 0;
}
