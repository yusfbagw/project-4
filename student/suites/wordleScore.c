#include "../assembly.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_wordle_score, .timeout = UNREASONABLY_LONG);

Test(test_wordle_score, null_guess) {
    char solution[] = "trace";
    int actual = wordleScore(NULL, solution);
    int expected = FAILURE;
    cr_assert(eq(int, actual, expected), "Expected NULL guess to return %d but got %d", expected, actual);
}

Test(test_wordle_score, null_solution) {
    char guess[] = "trace";
    int actual = wordleScore(guess, NULL);
    int expected = FAILURE;
    cr_assert(eq(int, actual, expected), "Expected NULL solution to return %d but got %d", expected, actual);
}

Test(test_wordle_score, invalid_guess_length) {
    char guess[] = "hi";
    char solution[] = "trace";
    int actual = wordleScore(guess, solution);
    int expected = FAILURE;
    cr_assert(eq(int, actual, expected), "Expected invalid length guess to return %d but got %d", expected, actual);
}

Test(test_wordle_score, invalid_solution_length) {
    char guess[] = "trace";
    char solution[] = "hi";
    int actual = wordleScore(guess, solution);
    int expected = FAILURE;
    cr_assert(eq(int, actual, expected), "Expected invalid length solution to return %d but got %d", expected, actual);
}

Test(test_wordle_score, perfect) {
    char guess[] = "trace";
    char solution[] = "trace";
    int actual = wordleScore(guess, solution);
    int expected = 50;
    cr_assert(eq(int, actual, expected), "Expected perfect match score %d but got %d", expected, actual);
}

Test(test_wordle_score, no_matches) {
    char guess[] = "abcde";
    char solution[] = "fghij";
    int actual = wordleScore(guess, solution);
    int expected = 0;
    cr_assert(eq(int, actual, expected), "Expected no match score %d but got %d", expected, actual);
}

Test(test_wordle_score, ok_attempt) {
    char guess[] = "flame";
    char solution[] = "eagle";
    int actual = wordleScore(guess, solution);
    int expected = 16;
    cr_assert(eq(int, actual, expected), "Expected ok attempt score %d but got %d", expected, actual);
}

Test(test_wordle_score, good_attempt) {
    char guess[] = "olleh";
    char solution[] = "hello";
    int actual = wordleScore(guess, solution);
    int expected = 22;
    cr_assert(eq(int, actual, expected), "Expected good attempt score %d but got %d", expected, actual);
}

Test(test_wordle_score, almost_perfect_attempt) {
    char guess[] = "hella";
    char solution[] = "hello";
    int actual = wordleScore(guess, solution);
    int expected = 40;
    cr_assert(eq(int, actual, expected), "Expected almost perfect score %d but got %d", expected, actual);
}
