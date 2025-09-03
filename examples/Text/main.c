#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "text.h"

/* Enhanced macro to print Text objects with detailed info */
#define PRINT_TEXT(txt, name) do { \
    if (txt) { \
        char *str = Text_toCStr(txt); \
        printf("%s: Length=%zu, Content=\"%s\"\n", \
               name, Text_length(txt), str ? str : "(null)"); \
        free(str); \
    } else { \
        printf("%s: NULL\n", name); \
    } \
} while(0)

/* Macro to print TextBuffer objects */
#define PRINT_TEXTBUFFER(buf, name) do { \
    if (buf) { \
        Text txt = TB2TXT(buf); \
        char *str = Text_toCStr(txt); \
        printf("%s: Length=%zu, Content=\"%s\"\n", \
               name, TextBuffer_length(buf), str ? str : "(null)"); \
        free(str); \
    } else { \
        printf("%s: NULL\n", name); \
    } \
} while(0)

#define SEPARATOR() printf("\n==============================================\n")

int main(void) {
    printf("BTCHWRK Text API Complete Test Suite\n");
    SEPARATOR();
    
    /* Test 1: Basic Text creation and macros */
    printf("1. Testing Text creation macros\n");
    Text txt1 = TXT("Hello World");
    PRINT_TEXT(txt1, "txt1 from TXT()");
    
    char *cstr = "Dynamic String";
    Text txt2 = CSTR2TXT(cstr);
    PRINT_TEXT(txt2, "txt2 from CSTR2TXT()");
    
    /* Test 2: Text access methods */
    printf("\n2. Testing Text access methods\n");
    Text sample = TXT("ABCDEFG");
    PRINT_TEXT(sample, "sample text");
    
    printf("charAt(0): '%c'\n", Text_charAt(sample, 0));
    printf("charAt(3): '%c'\n", Text_charAt(sample, 3));
    printf("charAt(6): '%c'\n", Text_charAt(sample, 6));
    printf("length(): %zu\n", Text_length(sample));
    
    /* Test 3: Text comparison */
    printf("\n3. Testing Text comparison\n");
    Text cmp1 = TXT("Apple");
    Text cmp2 = TXT("Banana");
    Text cmp3 = TXT("Apple");
    
    printf("compare(\"Apple\", \"Banana\"): %d\n", Text_compare(cmp1, cmp2));
    printf("compare(\"Apple\", \"Apple\"): %d\n", txtcmp(cmp1, cmp3));
    printf("compare(\"Banana\", \"Apple\"): %d\n", Text_compare(cmp2, cmp1));
    
    /* Test 4: Text search methods */
    printf("\n4. Testing Text search methods\n");
    Text haystack = TXT("The quick brown fox jumps over the lazy dog");
    Text needle1 = TXT("fox");
    Text needle2 = TXT("cat");
    Text needle3 = TXT("the");
    
    PRINT_TEXT(haystack, "haystack");
    printf("find(\"fox\"): %d\n", Text_find(haystack, needle1));
    printf("find(\"cat\"): %d\n", Text_find(haystack, needle2));
    printf("find(\"the\"): %d\n", Text_find(haystack, needle3));
    printf("rfind(\"the\"): %d\n", Text_rfind(haystack, needle3));
    printf("findChar('o'): %d\n", Text_findChar(haystack, 'o'));
    printf("findChar('z'): %d\n", Text_findChar(haystack, 'z'));
    
    /* Test 5: Text substring */
    printf("\n5. Testing Text substring\n");
    Text original = TXT("Hello, World!");
    PRINT_TEXT(original, "original");
    
    Text sub1 = Text_substring(original, 0, 5);
    Text sub2 = Text_substring(original, 7, 5);
    Text sub3 = Text_substring(original, 7, 100); /* Test bounds */
    
    PRINT_TEXT(sub1, "substring(0, 5)");
    PRINT_TEXT(sub2, "substring(7, 5)");
    PRINT_TEXT(sub3, "substring(7, 100)");
    
    /* Test 6: TextBuffer creation */
    printf("\n6. Testing TextBuffer creation\n");
    TextBuffer buf1 = TextBuffer_new(100);
    TextBuffer buf2 = TextBuffer_fromText(TXT("Initial content"));
    TextBuffer buf3 = TextBuffer_fromCStr("From C string");
    
    PRINT_TEXTBUFFER(buf1, "Empty buffer");
    PRINT_TEXTBUFFER(buf2, "Buffer from Text");
    PRINT_TEXTBUFFER(buf3, "Buffer from C string");
    
    /* Test 7: TextBuffer manipulation */
    printf("\n7. Testing TextBuffer manipulation\n");
    TextBuffer working_buf = TextBuffer_new(200);
    PRINT_TEXTBUFFER(working_buf, "Initial empty buffer");
    
    /* Test concat */
    TextBuffer_concat(working_buf, TXT("Hello"));
    PRINT_TEXTBUFFER(working_buf, "After concat 'Hello'");
    
    TextBuffer_concat(working_buf, TXT(" World"));
    PRINT_TEXTBUFFER(working_buf, "After concat ' World'");
    
    /* Test nConcat */
    TextBuffer_nConcat(working_buf, TXT("1234567890"), 3);
    PRINT_TEXTBUFFER(working_buf, "After nConcat '123...' (3 chars)");
    
    /* Test insert */
    TextBuffer_insert(working_buf, 5, TXT("***"));
    PRINT_TEXTBUFFER(working_buf, "After insert '***' at position 5");
    
    /* Test setChar */
    TextBuffer_setChar(working_buf, '!', TextBuffer_length(working_buf) - 1);
    PRINT_TEXTBUFFER(working_buf, "After setChar '!' at last position");
    
    /* Test 8: TextBuffer clear and reuse */
    printf("\n8. Testing TextBuffer clear and reuse\n");
    PRINT_TEXTBUFFER(working_buf, "Before clear");
    TextBuffer_clear(working_buf);
    PRINT_TEXTBUFFER(working_buf, "After clear");
    
    TextBuffer_concat(working_buf, TXT("Reused buffer"));
    PRINT_TEXTBUFFER(working_buf, "After reuse");
    
    /* Test 9: Edge cases */
    printf("\n9. Testing edge cases\n");
    
    /* Empty strings */
    Text empty = TXT("");
    PRINT_TEXT(empty, "Empty text");
    printf("Empty text length: %zu\n", Text_length(empty));
    
    TextBuffer empty_buf = TextBuffer_fromText(empty);
    PRINT_TEXTBUFFER(empty_buf, "Buffer from empty text");
    
    /* Special characters */
    Text special = TXT("Tab:\t, Newline:\n, Quote:\"");
    PRINT_TEXT(special, "Text with special chars");
    
    /* Test 10: Complex workflow */
    printf("\n10. Testing complex workflow\n");
    TextBuffer document = TextBuffer_new(1000);
    
    TextBuffer_concat(document, TXT("Title: My Document\n"));
    TextBuffer_concat(document, TXT("================\n\n"));
    TextBuffer_concat(document, TXT("Chapter 1: Introduction\n"));
    
    Text chapter_text = TXT("This is the introduction to our document. ");
    TextBuffer_concat(document, chapter_text);
    
    TextBuffer_nConcat(document, TXT("We will explore many topics in great detail throughout this work."), 25);
    
    PRINT_TEXTBUFFER(document, "Complete document");
    
    /* Extract and work with parts */
    Text doc_as_text = TB2TXT(document);
    int title_pos = Text_find(doc_as_text, TXT("Title:"));
    int chapter_pos = Text_find(doc_as_text, TXT("Chapter"));
    
    printf("Position of 'Title:': %d\n", title_pos);
    printf("Position of 'Chapter': %d\n", chapter_pos);
    
    /* Test 11: Performance test */
    printf("\n11. Performance test - building long text\n");
    TextBuffer perf_buf = TextBuffer_new(10000);
    
    for (int i = 0; i < 100; i++) {
        char temp[20];
        snprintf(temp, sizeof(temp), "Item%03d ", i);
        TextBuffer_concat(perf_buf, CSTR2TXT(temp));
    }
    
    printf("Performance buffer final length: %zu\n", TextBuffer_length(perf_buf));
    
    /* Show first and last parts */
    Text perf_text = TB2TXT(perf_buf);
    Text first_part = Text_substring(perf_text, 0, 50);
    size_t len = Text_length(perf_text);
    Text last_part = Text_substring(perf_text, len > 50 ? len - 50 : 0, 50);
    
    PRINT_TEXT(first_part, "Performance buffer start");
    PRINT_TEXT(last_part, "Performance buffer end");
    
    /* Test 12: Text conversion round-trip */
    printf("\n12. Testing Text ↔ C string conversion\n");
    char original_cstr[] = "Round trip test!";
    Text from_cstr = CSTR2TXT(original_cstr);
    char *back_to_cstr = TXT2STR(from_cstr);
    
    printf("Original C string: \"%s\"\n", original_cstr);
    PRINT_TEXT(from_cstr, "As Text");
    printf("Back to C string: \"%s\"\n", back_to_cstr);
    printf("Strings match: %s\n", strcmp(original_cstr, back_to_cstr) == 0 ? "YES" : "NO");
    
    free(back_to_cstr);
    
    /* Test 13: Boundary testing */
    printf("\n13. Testing boundary conditions\n");
    
    Text boundary_text = TXT("0123456789");
    printf("Original: ");
    PRINT_TEXT(boundary_text, "");
    
    /* Test charAt at boundaries */
    printf("charAt(0): '%c'\n", Text_charAt(boundary_text, 0));
    printf("charAt(9): '%c'\n", Text_charAt(boundary_text, 9));
    
    /* Test substring at boundaries */
    Text sub_start = Text_substring(boundary_text, 0, 1);
    Text sub_end = Text_substring(boundary_text, 9, 1);
    Text sub_all = Text_substring(boundary_text, 0, 10);
    
    PRINT_TEXT(sub_start, "substring(0,1)");
    PRINT_TEXT(sub_end, "substring(9,1)");
    PRINT_TEXT(sub_all, "substring(0,10)");
    
    /* Cleanup */
    printf("\n14. Cleanup\n");
    
    /* Free TextBuffers */
    TextBuffer_free(buf1);
    TextBuffer_free(buf2);
    TextBuffer_free(buf3);
    TextBuffer_free(working_buf);
    TextBuffer_free(empty_buf);
    TextBuffer_free(document);
    TextBuffer_free(perf_buf);
    
    /* Note: Text objects created with TXT() macro are stack-allocated
       and don't need explicit freeing. Only dynamically allocated
       Text objects (like from Text_substring) need to be freed. */
    if (first_part != NULL) free((void*)first_part);
    if (last_part  != NULL) free((void*)last_part);
    if (sub_start  != NULL) free((void*)sub_start);
    if (sub_end    != NULL) free((void*)sub_end);
    if (sub_all    != NULL) free((void*)sub_all);
    
    printf("Cleanup completed\n");
    
    SEPARATOR();
    printf("  All Text API tests completed successfully!\n");
    printf("  Tests covered:\n");
    printf("   - Text creation and macros\n");
    printf("   - Character access and length\n");
    printf("   - Text comparison\n");
    printf("   - Text searching (find, rfind, findChar)\n");
    printf("   - Text substring extraction\n");
    printf("   - TextBuffer creation and manipulation\n");
    printf("   - TextBuffer insertion and modification\n");
    printf("   - Edge cases and boundary conditions\n");
    printf("   - Performance testing\n");
    printf("   - Memory management\n");
    
