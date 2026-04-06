#include "../assembly.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_octal_string_to_int, .timeout = UNREASONABLY_LONG);
//Test invalid arguments
Test(test_octal_string_to_int, invalid_character) {
    char octalString[] = "1A";
    int length = 2;
    int result = octalStringToInt(octalString, length);
    cr_assert_eq(result, FAILURE, "Expected FAILURE for invalid character but got %d", result);
}
//Null arguments
Test(test_octal_string_to_int, null_arguments) {
    int result = octalStringToInt(NULL, 2);
    cr_assert_eq(result, FAILURE, "Expected FAILURE for NULL string but got %d", result);
    
    char octalString[] = "17";
    result = octalStringToInt(octalString, 0);
    cr_assert_eq(result, FAILURE, "Expected FAILURE for non-positive length but got %d", result);
}
//Test valid octal string
Test(test_octal_string_to_int, valid_octal_string) {
    char octalString[] = "17";
    int length = 2;
    int result = octalStringToInt(octalString, length);
    cr_assert_eq(result, 15, "Expected 15 but got %d", result);
}
//Test octal string with leading zeros
Test(test_octal_string_to_int, leading_zeros) {
    char octalString[] = "007";
    int length = 3;
    int result = octalStringToInt(octalString, length);
    cr_assert_eq(result, 7, "Expected 7 but got %d", result);
}
//Test octal string with multiple digits
Test(test_octal_string_to_int, multiple_digits) {
    char octalString[] = "1234567";
    int length = 7;
    int result = octalStringToInt(octalString, length);
    cr_assert_eq(result, 342391, "Expected 342391 but got %d", result);
}