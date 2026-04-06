#include "../assembly.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_longest_wordle_streak, .timeout = UNREASONABLY_LONG);

Test(test_longest_wordle_streak, null_array) {
    int expected = FAILURE;
    int actual = longestWordleStreak(NULL, 1);
    cr_assert(eq(int, actual, expected), "Expected null array to return %d but got %d", expected, actual);
}

Test(test_longest_wordle_streak, negative_length) {
    int arr[] = {0};
    int expected = FAILURE;
    int actual = longestWordleStreak(arr, -1);
    cr_assert(eq(int, actual, expected), "Expected negative length to return %d but got %d", expected, actual);
}

Test(test_longest_wordle_streak, all_zeros) {
    int arr[] = {0, 0, 0, 0, 0};
    int expected = 0;
    int actual = longestWordleStreak(arr, 5);
    cr_assert(eq(int, actual, expected), "Expected longest streak %d for all losses but got %d", expected, actual);
}

Test(test_longest_wordle_streak, all_ones) {
    int arr[] = {1, 1, 1, 1, 1, 1};
    int expected = 6;
    int actual = longestWordleStreak(arr, 6);
    cr_assert(eq(int, actual, expected), "Expected longest streak %d for all wins but got %d", expected, actual);
}

Test(test_longest_wordle_streak, mixed) {
    int arr[] = {1, 1, 0, 1, 1, 1, 0, 1};
    int expected = 3;
    int actual = longestWordleStreak(arr, 8);
    cr_assert(eq(int, actual, expected), "Expected longest streak %d for mixed results but got %d", expected, actual);
}

Test(test_longest_wordle_streak, mixed_again) {
    int arr[] = {1, 1, 0, 1, 1, 0, 0, 0, 0};
    int expected = 2;
    int actual = longestWordleStreak(arr, 9);
    cr_assert(eq(int, actual, expected), "Expected longest streak %d for mixed (again) results but got %d", expected, actual);
}
