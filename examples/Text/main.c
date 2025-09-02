#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "text.h"


#define PRINT_TEXT(txt, name) do { \
    char *str = Text_toString(txt); \
    printf("%s: Length=%zu, Content=\"%s\"\n", \
           name, Text_length(txt), str ? str : "(null)"); \
    Text_free(str);\
} while(0)

#define SEPARATOR() printf("\n==============================================\n")


int main(void) {
    printf("BTCHWRK Text API Complete Test\n");
    SEPARATOR();
    
    // Test 1: Constructor with TXT macro
    printf("1. Testing TXT() macro and Text_new()\n");
    Text txt1 = TXT("Hello");
    PRINT_TEXT(txt1, "txt1 from TXT(\"Hello\")");
    
    // Test 2: Manual constructor
    printf("\n2. Testing Text_new() manually\n");
    char *manual_str = "World!";
    Text txt2 = Text_new(manual_str, strlen(manual_str));
    PRINT_TEXT(txt2, "txt2 from manual Text_new");
    
    // Test 3: Length function
    printf("\n3. Testing Text_length()\n");
    printf("Length of txt1: %zu\n", Text_length(txt1));
    printf("Length of txt2: %zu\n", Text_length(txt2));
    
    // Test 4: toString function
    printf("\n4. Testing Text_toString()\n");
    char *str1 = Text_toString(txt1);
    char *str2 = Text_toString(txt2);
    printf("txt1 as string: \"%s\"\n", str1);
    printf("txt2 as string: \"%s\"\n", str2);
    free(str1);
    free(str2);
    
    // Test 5: Comparison
    printf("\n5. Testing Text_compare() and txtcmp() macro\n");
    Text txt3 = TXT("Hello");
    Text txt4 = TXT("World");
    Text txt5 = TXT("Apple");
    
    printf("compare(\"Hello\", \"Hello\"): %d\n", Text_compare(txt1, txt3));
    printf("compare(\"Hello\", \"World\"): %d\n", txtcmp(txt1, txt4));
    printf("compare(\"Hello\", \"Apple\"): %d\n", Text_compare(txt1, txt5));
    printf("compare(\"Apple\", \"Hello\"): %d\n", Text_compare(txt5, txt1));
    
    // Test 6: Concatenation
    printf("\n6. Testing Text_concat() and txtcat() macro\n");
    Text txt6 = TXT("Hello");
    Text txt7 = TXT(" World");
    PRINT_TEXT(txt6, "Before concat - txt6");
    PRINT_TEXT(txt7, "Before concat - txt7");
    
    txtcat(txt6, txt7);
    PRINT_TEXT(txt6, "After txtcat(txt6, txt7)");
    
    // Test 7: N-Concatenation (partial)
    printf("\n7. Testing Text_nConcat() and txtncat() macro\n");
    Text txt8 = TXT("Start");
    Text txt9 = TXT("1234567890");
    PRINT_TEXT(txt8, "Before nConcat - txt8");
    PRINT_TEXT(txt9, "Before nConcat - txt9");
    
    Text_nConcat(txt8, txt9, 5);  // Only concat first 5 chars
    PRINT_TEXT(txt8, "After nConcat(txt8, txt9, 5)");
    
    // Test 8: Empty text handling
    printf("\n8. Testing empty text handling\n");
    Text empty1 = TXT("");
    Text empty2 = Text_new(NULL, 0);  // Test NULL string
    PRINT_TEXT(empty1, "Empty from TXT(\"\")");
    if (empty2) {
        PRINT_TEXT(empty2, "Empty from Text_new(NULL, 0)");
    } else {
        printf("Text_new(NULL, 0) returned NULL\n");
    }
    
    // Test 9: Long text handling
    printf("\n9. Testing long text\n");
    char long_string[1000];
    for (int i = 0; i < 999; i++) {
        long_string[i] = 'A' + (i % 26);
    }
    long_string[999] = '\0';
    
    Text long_txt = Text_new(long_string, 999);
    printf("Long text length: %zu\n", Text_length(long_txt));
    
    char *long_str = Text_toString(long_txt);
    printf("First 50 chars: \"%.50s\"\n", long_str);
    printf("Last 50 chars: \"%.50s\"\n", long_str + Text_length(long_txt) - 50);
    free(long_str);
    
    // Test 10: Multiple concatenations
    printf("\n10. Testing multiple concatenations\n");
    Text multi = TXT("A");
    Text add_b = TXT("B");
    Text add_c = TXT("C");
    Text add_d = TXT("D");
    
    PRINT_TEXT(multi, "Start");
    Text_concat(multi, add_b);
    PRINT_TEXT(multi, "After +B");
    Text_concat(multi, add_c);
    PRINT_TEXT(multi, "After +C");
    Text_concat(multi, add_d);
    PRINT_TEXT(multi, "Final");
    
    // Test 11: Self-operations (edge cases)
    printf("\n11. Testing edge cases\n");
    Text self_test = TXT("Self");
    printf("Self-comparison: %d (should be 0)\n", Text_compare(self_test, self_test));
    
    // Test with special characters
    Text special = TXT("Hello\nWorld\tTest!");
    PRINT_TEXT(special, "Text with special chars");
    
    // Test 12: Mixed operations
    printf("\n12. Testing mixed operations\n");
    Text mixed1 = TXT("The quick ");
    Text mixed2 = TXT("brown fox ");
    Text mixed3 = TXT("jumps over the lazy dog");
    
    txtcat(mixed1, mixed2);
    txtncat(mixed1, mixed3, 10);  // Only add "jumps over"
    PRINT_TEXT(mixed1, "Final mixed result");
    
    // Test 13: Performance test (many small operations)
    printf("\n13. Testing performance with many operations\n");
    Text perf_test = TXT("");
    Text digit = TXT("X");
    
    for (int i = 0; i < 100; i++) {
        Text_concat(perf_test, digit);
    }
    printf("After 100 concatenations: length = %zu\n", Text_length(perf_test));
    
    // Cleanup
    printf("\n14. Cleanup\n");
    Text_free(txt1);
    Text_free(txt2);
    Text_free(txt3);
    Text_free(txt4);
    Text_free(txt5);
    Text_free(txt6);
    Text_free(txt7);
    Text_free(txt8);
    Text_free(txt9);
    if (empty1) Text_free(empty1);
    if (empty2) Text_free(empty2);
    Text_free(long_txt);
    Text_free(multi);
    Text_free(add_b);
    Text_free(add_c);
    Text_free(add_d);
    Text_free(self_test);
    Text_free(special);
    Text_free(mixed1);
    Text_free(mixed2);
    Text_free(mixed3);
    Text_free(perf_test);
    Text_free(digit);
    
    printf("All text objects freed successfully\n");
    
    SEPARATOR();
    printf("✅ All Text API tests completed!\n");
    
    return 0;
}
