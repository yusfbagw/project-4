#include "../assembly.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_valid_sudoku_row, .timeout = UNREASONABLY_LONG);

Test(test_valid_sudoku_row, null_row) {
    int actual = validSudokuRow(NULL);
    int expected = FAILURE;
    cr_assert(eq(int, actual, expected), "Expected null row to return %d but got %d", expected, actual);
}

Test(test_valid_sudoku_row, short_row) {
    char row[] = "12345678";
    int actual = validSudokuRow(row);
    int expected = FAILURE;
    cr_assert(eq(int, actual, expected), "Expected short row to return %d but got %d", expected, actual);
}

Test(test_valid_sudoku_row, long_row) {
    char row[] = "1234567899";
    int actual = validSudokuRow(row);
    int expected = FAILURE;
    cr_assert(eq(int, actual, expected), "Expected long row to return %d but got %d", expected, actual);
}

Test(test_valid_sudoku_row, invalid_characters) {
    char row[] = "ABCDEFGHI";
    int actual = validSudokuRow(row);
    int expected = FAILURE;
    cr_assert(eq(int, actual, expected), "Expected invalid row to return %d but got %d", expected, actual);
}

Test(test_valid_sudoku_row, one_invalid_character) {
    char row[] = "12345678A";
    int actual = validSudokuRow(row);
    int expected = FAILURE;
    cr_assert(eq(int, actual, expected), "Expected row one invalid character to return %d but got %d", expected, actual);
}

Test(test_valid_sudoku_row, duplicate_digit) {
    char row[] = "553678912";
    int actual = validSudokuRow(row);
    int expected = FAILURE;
    cr_assert(eq(int, actual, expected), "Expected duplicate row to return %d but got %d", expected, actual);
}

Test(test_valid_sudoku_row, valid_row) {
    char row[] = "534678912";
    int actual = validSudokuRow(row);
    int expected = SUCCESS;
    cr_assert(eq(int, actual, expected), "Expected valid row to return %d but got %d", expected, actual);
}

Test(test_valid_sudoku_row, valid_row_again) {
    char row[] = "294687513";
    int actual = validSudokuRow(row);
    int expected = SUCCESS;
    cr_assert(eq(int, actual, expected), "Expected valid row (again) to return %d but got %d", expected, actual);
}
