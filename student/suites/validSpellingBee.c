#include "../assembly.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_valid_spelling_bee, .timeout = UNREASONABLY_LONG);

//Test edges cases for validSpellingBee; invalid unll arguments
Test(test_valid_spelling_bee, null_arguments) {
  cr_assert_eq(validSpellingBee(NULL, "ABC", 'A'), FAILURE, "Expected NULL word to be invalid but got %d", validSpellingBee(NULL, "ABC", 'A'));
  cr_assert_eq(validSpellingBee("WORD", NULL, 'A'), FAILURE, "Expected NULL alphabet to be invalid but got %d", validSpellingBee("WORD", NULL, 'A'));
  cr_assert_eq(validSpellingBee("WORD", "ABC", '\0'), FAILURE, "Expected null center char to be invalid but got %d", validSpellingBee("WORD", "ABC", '\0'));
  cr_assert_eq(validSpellingBee("", "", 'A'), FAILURE, "Expected all NULL arguments to be invalid but got %d", validSpellingBee(NULL, NULL, '\0'));
}

//Test valid cases
Test(test_valid_spelling_bee, valid_cases) {
  cr_assert_eq(validSpellingBee("CAB", "ABC", 'A'), SUCCESS, "Expected %d but got %d", helpervalidSpellingBee("CAB", "ABC", 'A'), validSpellingBee("CAB", "ABC", 'A'));
  cr_assert_eq(validSpellingBee("AAA", "ABC", 'A'), SUCCESS, "Expected %s but got %s", helpervalidSpellingBee("AAA", "ABC", 'A') ? "valid" : "invalid", validSpellingBee("AAA", "ABC", 'A') ? "valid" : "invalid");
  cr_assert_eq(validSpellingBee("BAC", "ABC", 'A'), SUCCESS, "Expected %d but got %d", helpervalidSpellingBee("BAC", "ABC", 'A'), validSpellingBee("BAC", "ABC", 'A'));
}

//Test invalid cases
Test(test_valid_spelling_bee, invalid_cases) {
  cr_assert_eq(validSpellingBee("CAB", "AB", 'A'), FAILURE, "Expected %d but got %d", helpervalidSpellingBee("CAB", "AB", 'A'), validSpellingBee("CAB", "AB", 'A'));
  cr_assert_eq(validSpellingBee("CAB", "R", 'B'), FAILURE, "Expected %d but got %d", helpervalidSpellingBee("CAB", "ABC", 'B'), validSpellingBee("CAB", "ABC", 'B'));
  cr_assert_eq(validSpellingBee("CAB", "ABC", 'D'), FAILURE, "Expected %d but got %d", helpervalidSpellingBee("CAB", "ABC", 'D'), validSpellingBee("CAB", "ABC", 'D'));
}

