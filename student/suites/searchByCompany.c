#include "../humanresources.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

static void setup(void) {
    helper_reset_applications();
}

TestSuite(test_search_by_company, .timeout = UNREASONABLY_LONG, .init = setup);

Test(test_search_by_company, null_company) {
    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByCompany(NULL, results);
    cr_assert(eq(int, ret, ERROR),
        "searchByCompany with NULL company should return ERROR, got %d", ret);
}

Test(test_search_by_company, empty_company) {
    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByCompany("", results);
    cr_assert(eq(int, ret, ERROR),
        "searchByCompany with empty company should return ERROR, got %d", ret);
}

Test(test_search_by_company, company_too_long) {
    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    char long_company[MAX_COMPANY_LEN + 10];
    for (int i = 0; i < MAX_COMPANY_LEN + 5; i++) {
        long_company[i] = 'A';
    }
    long_company[MAX_COMPANY_LEN + 5] = '\0';
    int ret = searchByCompany(long_company, results);
    cr_assert(eq(int, ret, ERROR),
        "searchByCompany with company name too long should return ERROR, got %d", ret);
}

Test(test_search_by_company, no_applications) {
    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByCompany("Google", results);
    cr_assert(eq(int, ret, 0),
        "searchByCompany with no applications should return 0, got %d", ret);
}

Test(test_search_by_company, no_match) {
    helper_add_app("John", "Doe", 25, 5, 0);
    helper_add_exp(0, "Apple", 2020, 2023, ENGINEER_JUNIOR);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByCompany("Google", results);
    cr_assert(eq(int, ret, 0),
        "searchByCompany should return 0 when no applicant works at the company, got %d", ret);
}

Test(test_search_by_company, no_experiences) {
    helper_add_app("John", "Doe", 25, 5, 0);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByCompany("Google", results);
    cr_assert(eq(int, ret, 0),
        "Applicant with no experiences should not match any company, got %d", ret);
}

Test(test_search_by_company, single_match) {
    helper_add_app("John", "Doe", 25, 5, 0);
    helper_add_exp(0, "Google", 2020, 2023, ENGINEER_JUNIOR);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByCompany("Google", results);
    cr_assert(eq(int, ret, 1),
        "searchByCompany should return 1 for single match, got %d", ret);

    char expected[MAX_APPLICANT_NAME_LEN * 2 + 2];
    helper_format_name(expected, "John", "Doe");
    cr_assert(eq(str, results[0], expected),
        "Result should contain 'John Doe'");
}

Test(test_search_by_company, multiple_matches) {
    helper_add_app("John", "Doe", 25, 5, 0);
    helper_add_exp(0, "Google", 2020, 2023, ENGINEER_JUNIOR);

    helper_add_app("Jane", "Smith", 30, 3, 1);
    helper_add_exp(1, "Google", 2018, 2022, ENGINEER_SENIOR);

    helper_add_app("Bob", "Jones", 28, 4, 0);
    helper_add_exp(2, "Apple", 2019, 2023, MANAGER);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByCompany("Google", results);
    cr_assert(eq(int, ret, 2),
        "searchByCompany should return 2 for two matches, got %d", ret);

    char expected0[MAX_APPLICANT_NAME_LEN * 2 + 2];
    char expected1[MAX_APPLICANT_NAME_LEN * 2 + 2];
    helper_format_name(expected0, "John", "Doe");
    helper_format_name(expected1, "Jane", "Smith");
    cr_assert(eq(str, results[0], expected0),
        "First result should be 'John Doe'");
    cr_assert(eq(str, results[1], expected1),
        "Second result should be 'Jane Smith'");
}

Test(test_search_by_company, duplicate_company_counted_once) {
    helper_add_app("John", "Doe", 25, 5, 0);
    helper_add_exp(0, "Google", 2018, 2020, INTERN);
    helper_add_exp(0, "Google", 2020, 2023, ENGINEER_JUNIOR);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByCompany("Google", results);
    cr_assert(eq(int, ret, 1),
        "Applicant with multiple experiences at same company should be counted once, got %d", ret);
}

Test(test_search_by_company, all_match) {
    helper_add_app("John", "Doe", 25, 5, 0);
    helper_add_exp(0, "Google", 2020, 2023, ENGINEER_JUNIOR);

    helper_add_app("Jane", "Smith", 30, 3, 1);
    helper_add_exp(1, "Google", 2018, 2022, ENGINEER_SENIOR);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByCompany("Google", results);
    cr_assert(eq(int, ret, 2),
        "searchByCompany should return 2 when all applicants match, got %d", ret);
}

Test(test_search_by_company, mixed_companies) {
    helper_add_app("Alice", "Brown", 26, 4, 0);
    helper_add_exp(0, "Meta", 2019, 2021, INTERN);
    helper_add_exp(0, "Google", 2021, 2023, ENGINEER_JUNIOR);

    helper_add_app("Charlie", "White", 32, 6, 1);
    helper_add_exp(1, "Amazon", 2015, 2020, MANAGER);
    helper_add_exp(1, "Google", 2020, 2024, MANAGER_SENIOR);

    helper_add_app("Diana", "Green", 29, 2, 0);
    helper_add_exp(2, "Meta", 2020, 2023, ENGINEER_SENIOR);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByCompany("Google", results);
    cr_assert(eq(int, ret, 2),
        "Should find 2 applicants who worked at Google, got %d", ret);

    char expected0[MAX_APPLICANT_NAME_LEN * 2 + 2];
    char expected1[MAX_APPLICANT_NAME_LEN * 2 + 2];
    helper_format_name(expected0, "Alice", "Brown");
    helper_format_name(expected1, "Charlie", "White");
    cr_assert(eq(str, results[0], expected0),
        "First result should be 'Alice Brown'");
    cr_assert(eq(str, results[1], expected1),
        "Second result should be 'Charlie White'");
}
