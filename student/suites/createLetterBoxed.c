#include "../assembly.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_create_letter_boxed, .timeout = UNREASONABLY_LONG);

Test(test_create_letter_boxed, null_array) {
    createLetterBoxed(NULL, 1);
    cr_assert(1, "Should not crash on null");
}

Test(test_create_letter_boxed, null_in_array) {
    char *str[] = {"hi", NULL, "bye"};
    createLetterBoxed(str, 3);
    cr_assert(1, "Should not crash on null in array");
}

Test(test_create_letter_boxed, negative_length) {
    char s1[] = "hi";
    char s2[] = "bye";
    char *actual[] = {s1, s2};
    char *expected[] = {s1, s2};

    createLetterBoxed(actual, -1);
    cr_assert(1, "Should not crash on negative length");
    cr_assert(eq(ptr, actual[0], expected[0]), "Expected first word unchanged for negative length");
    cr_assert(eq(ptr, actual[1], expected[1]), "Expected second word unchanged for negative length");
}

Test(test_create_letter_boxed, single_string_unchanged) {
    char s1[] = "fade";
    char *actual[] = {s1};
    char *expected[] = {s1};

    createLetterBoxed(actual, 1);
    cr_assert(eq(ptr, actual[0], expected[0]), "Expected single element pointer unchanged");
    cr_assert(eq(str, actual[0], expected[0]), "Expected single element string unchanged");
}

Test(test_create_letter_boxed, simple) {
    char s1[] = "fade";
    char s2[] = "brick";
    char *actual[] = {s1, s2};
    char *expected[] = {"fade", "erick"};

    createLetterBoxed(actual, 2);
    cr_assert(eq(str, actual[0], expected[0]), "Expected first word to be \"%s\"", expected[0]);
    cr_assert(eq(str, actual[1], expected[1]), "Expected second word to be \"%s\"", expected[1]);
}

Test(test_create_letter_boxed, advanced) {
    char s0[] = "i";
    char s1[] = "wonder";
    char s2[] = "what";
    char s3[] = "this";
    char s4[] = "will";
    char s5[] = "be";
    char *actual[] = {s0, s1, s2, s3, s4, s5};
    char *expected[] = {"i", "ionder", "rhat", "this", "sill", "le"};

    createLetterBoxed(actual, 6);
    cr_assert(eq(str, actual[0], expected[0]), "Expected first word to be \"%s\"", expected[0]);
    cr_assert(eq(str, actual[1], expected[1]), "Expected second word to be \"%s\"", expected[1]);
    cr_assert(eq(str, actual[2], expected[2]), "Expected third word to be \"%s\"", expected[2]);
    cr_assert(eq(str, actual[3], expected[3]), "Expected fourth word to be \"%s\"", expected[3]);
    cr_assert(eq(str, actual[4], expected[4]), "Expected fifth word to be \"%s\"", expected[4]);
    cr_assert(eq(str, actual[5], expected[5]), "Expected sixth word to be \"%s\"", expected[5]);
}
