#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "text.h"

void test_text_basics() {
    printf("=== Testing Text Basics ===\n");
    
    // Test TXT macro
    Text hello = TXT("Hello");
    Text world = TXT(" World!");
    
    printf("Text 1: '%.*s' (length: %zu)\n", (int)hello.length, hello.data, hello.length);
    printf("Text 2: '%.*s' (length: %zu)\n", (int)world.length, world.data, world.length);
    
    // Test CSTR2TXT macro
    const char *cstr = "From C String";
    Text from_cstr = CSTR2TXT(cstr);
    printf("From C String: '%.*s' (length: %zu)\n", (int)from_cstr.length, from_cstr.data, from_cstr.length);
    
    // Test Text_charAt
    printf("Character at index 1 in hello: '%c'\n", Text_charAt(hello, 1));
    
    // Test Text_length
    printf("Length of hello: %zu\n", Text_length(hello));
    
    // Test Text_findChar
    int pos = Text_findChar(hello, 'l');
    printf("First 'l' in hello at position: %d\n", pos);
    
    // Test Text_compare
    Text hello2 = TXT("Hello");
    Text goodbye = TXT("Goodbye");
    printf("Compare hello with hello2: %d\n", Text_compare(hello, hello2));
    printf("Compare hello with goodbye: %d\n", Text_compare(hello, goodbye));
    
    printf("\n");
}

void test_text_search() {
    printf("=== Testing Text Search ===\n");
    
    Text sentence = TXT("The quick brown fox jumps over the lazy dog");
    Text needle1 = TXT("fox");
    Text needle2 = TXT("cat");
    Text needle3 = TXT("the");
    
    printf("Sentence: '%.*s'\n", (int)sentence.length, sentence.data);
    
    int pos1 = Text_find(sentence, needle1);
    int pos2 = Text_find(sentence, needle2);
    int pos3 = Text_find(sentence, needle3);
    int rpos3 = Text_rfind(sentence, needle3);
    
    printf("Find 'fox': %d\n", pos1);
    printf("Find 'cat': %d\n", pos2);
    printf("Find 'the': %d\n", pos3);
    printf("Reverse find 'the': %d\n", rpos3);
    
    printf("\n");
}

void test_textbuffer() {
    printf("=== Testing TextBuffer ===\n");
    
    // Test TextBuffer_new
    TextBuffer buf = TextBuffer_new(100);
    printf("Created new TextBuffer with capacity 100\n");
    printf("Initial length: %zu\n", TextBuffer_length(buf));
    
    // Test TextBuffer_concat
    Text hello = TXT("Hello");
    Text space = TXT(" ");
    Text world = TXT("World");
    
    TextBuffer_concat(buf, hello);
    printf("After adding 'Hello': '%s' (length: %zu)\n", TB2STR(buf), TextBuffer_length(buf));
    
    TextBuffer_concat(buf, space);
    TextBuffer_concat(buf, world);
    printf("After adding ' World': '%s' (length: %zu)\n", TB2STR(buf), TextBuffer_length(buf));
    
    // Test TextBuffer_insert
    Text beautiful = TXT("Beautiful ");
    TextBuffer_insert(buf, 6, beautiful);
    printf("After inserting 'Beautiful ' at position 6: '%s'\n", TB2STR(buf));
    
    // Test TextBuffer_setChar
    TextBuffer_setChar(buf, '!', TextBuffer_length(buf));
    printf("After adding '!' at end: '%s'\n", TB2STR(buf));
    
    // Test TextBuffer_substring
    TextBuffer sub = TextBuffer_substring(buf, 6, 9);
    printf("Substring from position 6, length 9: '%s'\n", TB2STR(sub));
    
    // Test TextBuffer_clear
    TextBuffer_clear(buf);
    printf("After clear: '%s' (length: %zu)\n", TB2STR(buf), TextBuffer_length(buf));
    
    // Cleanup
    TextBuffer_free(buf);
    TextBuffer_free(sub);
    printf("TextBuffer freed\n");
    
    printf("\n");
}

void test_textbuffer_from_existing() {
    printf("=== Testing TextBuffer from existing data ===\n");
    
    // Test TextBuffer_fromText
    Text original = TXT("Original Text");
    TextBuffer buf1 = TextBuffer_fromText(original);
    printf("TextBuffer from Text: '%s'\n", TB2STR(buf1));
    
    // Test TextBuffer_fromCStr
    const char *cstr = "C String Data";
    TextBuffer buf2 = TextBuffer_fromCStr(cstr);
    printf("TextBuffer from C String: '%s'\n", TB2STR(buf2));
    
    // Test nConcat
    Text extra = TXT(" with extra content that is quite long");
    TextBuffer_nConcat(buf1, extra, 11);  // Only add " with extra"
    printf("After nConcat (11 chars): '%s'\n", TB2STR(buf1));
    
    // Cleanup
    TextBuffer_free(buf1);
    TextBuffer_free(buf2);
    
    printf("\n");
}

void test_text_conversion() {
    printf("=== Testing Text Conversion ===\n");
    
    Text original = TXT("Test String");
    char *cstr = Text_toCStr(original);
    printf("Original Text: '%.*s'\n", (int)original.length, original.data);
    printf("Converted to C String: '%s'\n", cstr);
    
    // Remember to free the allocated string
    // free(cstr);  // Uncomment if Text_toCStr allocates memory
    
    printf("\n");
}

int main() {
    printf("Text API Test Program\n");
    printf("====================\n\n");
    
    test_text_basics();
    test_text_search();
    test_textbuffer();
    test_textbuffer_from_existing();
    test_text_conversion();
    
    printf("All tests completed!\n");
    printf("Note: This test assumes all functions work correctly.\n");
    printf("Add proper error checking and assertions for production use.\n");
    
    return 0;
}
