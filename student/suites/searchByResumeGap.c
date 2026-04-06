#include "../humanresources.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

static void setup(void) {
    helper_reset_applications();
}

TestSuite(test_search_by_resume_gap, .timeout = UNREASONABLY_LONG, .init = setup);

Test(test_search_by_resume_gap, null_results) {
    int ret = searchByResumeGap(NULL);
    cr_assert(eq(int, ret, ERROR),
        "searchByResumeGap with NULL results should return ERROR, got %d", ret);
}

Test(test_search_by_resume_gap, no_applications) {
    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByResumeGap(results);
    cr_assert(eq(int, ret, 0),
        "searchByResumeGap with no applications should return 0, got %d", ret);
}

Test(test_search_by_resume_gap, no_experiences) {
    helper_add_app("John", "Doe", 25, 5, 0);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByResumeGap(results);
    cr_assert(eq(int, ret, 0),
        "Applicant with no experiences should not have a resume gap, got %d", ret);
}

Test(test_search_by_resume_gap, one_experience) {
    helper_add_app("John", "Doe", 25, 5, 0);
    helper_add_exp(0, "Google", 2020, 2023, ENGINEER_JUNIOR);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByResumeGap(results);
    cr_assert(eq(int, ret, 0),
        "Applicant with one experience should not have a resume gap, got %d", ret);
}

Test(test_search_by_resume_gap, no_gap_consecutive) {
    helper_add_app("John", "Doe", 25, 5, 0);
    helper_add_exp(0, "Google", 2018, 2020, ENGINEER_JUNIOR);
    helper_add_exp(0, "Apple", 2020, 2023, ENGINEER_SENIOR);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByResumeGap(results);
    cr_assert(eq(int, ret, 0),
        "Applicant with consecutive experiences (no gap) should not be counted, got %d", ret);
}

Test(test_search_by_resume_gap, below_gap_boundary) {
    helper_add_app("John", "Doe", 25, 5, 0);
    helper_add_exp(0, "Google", 2015, 2018, ENGINEER_JUNIOR);
    helper_add_exp(0, "Apple", 2020, 2023, ENGINEER_SENIOR);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByResumeGap(results);
    cr_assert(eq(int, ret, 0),
        "Gap of 2 years should not be counted as a resume gap, got %d", ret);
}

Test(test_search_by_resume_gap, exact_gap_boundary) {
    helper_add_app("John", "Doe", 25, 5, 0);
    helper_add_exp(0, "Google", 2015, 2018, ENGINEER_JUNIOR);
    helper_add_exp(0, "Apple", 2021, 2023, ENGINEER_SENIOR);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByResumeGap(results);
    cr_assert(eq(int, ret, 1),
        "Gap of exactly 3 years should be counted as a resume gap, got %d", ret);

    char expected[MAX_APPLICANT_NAME_LEN * 2 + 2];
    helper_format_name(expected, "John", "Doe");
    cr_assert(eq(str, results[0], expected),
        "Result should contain 'John Doe'");
}

Test(test_search_by_resume_gap, large_gap) {
    helper_add_app("John", "Doe", 40, 5, 0);
    helper_add_exp(0, "Google", 2005, 2010, ENGINEER_JUNIOR);
    helper_add_exp(0, "Apple", 2020, 2023, ENGINEER_SENIOR);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByResumeGap(results);
    cr_assert(eq(int, ret, 1),
        "Large gap of 10 years should be counted, got %d", ret);
}

Test(test_search_by_resume_gap, multiple_applicants_mixed) {
    helper_add_app("John", "Doe", 30, 5, 0);
    helper_add_exp(0, "Google", 2010, 2015, ENGINEER_JUNIOR);
    helper_add_exp(0, "Apple", 2020, 2023, ENGINEER_SENIOR);

    helper_add_app("Jane", "Smith", 28, 3, 1);
    helper_add_exp(1, "Meta", 2018, 2020, INTERN);
    helper_add_exp(1, "Meta", 2020, 2023, ENGINEER_JUNIOR);

    helper_add_app("Bob", "Jones", 35, 4, 0);
    helper_add_exp(2, "Amazon", 2008, 2012, ENGINEER_JUNIOR);
    helper_add_exp(2, "Netflix", 2018, 2023, MANAGER);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByResumeGap(results);
    cr_assert(eq(int, ret, 2),
        "Should find 2 applicants with resume gaps, got %d", ret);
}

Test(test_search_by_resume_gap, unsorted_experiences) {
    helper_add_app("John", "Doe", 30, 5, 0);
    helper_add_exp(0, "Apple", 2020, 2023, ENGINEER_SENIOR);
    helper_add_exp(0, "Google", 2010, 2015, ENGINEER_JUNIOR);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByResumeGap(results);
    cr_assert(eq(int, ret, 1),
        "Should detect gap even when experiences are not in chronological order, got %d", ret);
}

Test(test_search_by_resume_gap, multiple_gaps_counted_once) {
    helper_add_app("John", "Doe", 40, 5, 0);
    helper_add_exp(0, "CompA", 2000, 2005, INTERN);
    helper_add_exp(0, "CompB", 2010, 2015, ENGINEER_JUNIOR);
    helper_add_exp(0, "CompC", 2020, 2023, ENGINEER_SENIOR);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByResumeGap(results);
    cr_assert(eq(int, ret, 1),
        "Applicant with multiple gaps should only be counted once, got %d", ret);
}

Test(test_search_by_resume_gap, overlapping_experiences) {
    helper_add_app("John", "Doe", 30, 5, 0);
    helper_add_exp(0, "Google", 2015, 2020, ENGINEER_JUNIOR);
    helper_add_exp(0, "Apple", 2018, 2023, ENGINEER_SENIOR);

    char results[MAX_APPLICATIONS_LEN][MAX_APPLICANT_NAME_LEN * 2 + 2];
    int ret = searchByResumeGap(results);
    cr_assert(eq(int, ret, 0),
        "Overlapping experiences should not count as a resume gap, got %d", ret);
}
