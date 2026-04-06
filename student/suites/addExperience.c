#include "../humanresources.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

TestSuite(test_add_experience, .timeout = UNREASONABLY_LONG);

/*
Preconditions not in PDF need to be checked(NULL args, no matching app -> FAILURE):
first_name, last_name, company_name must be non-NULL and non-empty
start_year and end_year must be >= 0
start_year must be <= end_year
end_year must be <= 2026
experience_type must be in [INTERN, CEO]
company_name length must be <= MAX_COMPANY_LEN
first_name / last_name lengths must be <= MAX_APPLICANT_NAME_LEN
If matching application is found but experiences array is full -> ERROR
*/

// NULL/invalid argument tests

Test(test_add_experience, null_first_name) {
    cr_assert(eq(int, addExperience(NULL, "Kapoor", "Google", 2018, 2022, MANAGER), ERROR), "NULL first_name should return ERROR");
}

Test(test_add_experience, null_last_name) {
    cr_assert(eq(int, addExperience("Tanay", NULL, "Google", 2018, 2022, MANAGER), ERROR), "NULL last_name should return ERROR");
}

Test(test_add_experience, null_company_name) {
    cr_assert(eq(int, addExperience("Tanay", "Kapoor", NULL, 2018, 2022, MANAGER), ERROR), "NULL company_name should return ERROR");
}

Test(test_add_experience, empty_first_name) {
    cr_assert(eq(int, addExperience("", "Kapoor", "Google", 2018, 2022, MANAGER), ERROR), "empty first_name should return ERROR");
}

Test(test_add_experience, empty_last_name) {
    cr_assert(eq(int, addExperience("Tanay", "", "Google", 2018, 2022, MANAGER), ERROR), "empty last_name should return ERROR");
}

Test(test_add_experience, empty_company_name) {
    cr_assert(eq(int, addExperience("Tanay", "Kapoor", "", 2018, 2022, MANAGER), ERROR), "empty company_name should return ERROR");
}

Test(test_add_experience, negative_start_year) {
    cr_assert(eq(int, addExperience("Tanay", "Kapoor", "Google", -1, 2022, MANAGER), ERROR), "negative start_year should return ERROR");
}

Test(test_add_experience, negative_end_year) {
    cr_assert(eq(int, addExperience("Tanay", "Kapoor", "Google", 2018, -1, MANAGER), ERROR), "negative end_year should return ERROR");
}

Test(test_add_experience, start_year_after_end_year) {
    cr_assert(eq(int, addExperience("Tanay", "Kapoor", "Google", 2022, 2018, MANAGER), ERROR), "start_year > end_year should return ERROR");
}

Test(test_add_experience, end_year_above_2026) {
    cr_assert(eq(int, addExperience("Tanay", "Kapoor", "Google", 2025, 2027, MANAGER), ERROR), "end_year > 2026 should return ERROR");
}

Test(test_add_experience, invalid_experience_type_negative) {
    cr_assert(eq(int, addExperience("Tanay", "Kapoor", "Google", 2018, 2022, (enum ExperienceType)(-1)), ERROR), "experience_type below INTERN should return ERROR");
}

Test(test_add_experience, invalid_experience_type_too_high) {
    cr_assert(eq(int, addExperience("Tanay", "Kapoor", "Google", 2018, 2022, (enum ExperienceType)(CEO + 1)), ERROR), "experience_type above CEO should return ERROR");
}

// No matching application -> FAILURE

Test(test_add_experience, no_applications_exist) {
    /* app_count starts at 0; no applications in array */
    cr_assert(eq(int, addExperience("Tanay", "Kapoor", "Google", 2018, 2022, MANAGER), FAILURE), "should return FAILURE when no applications exist");
}

Test(test_add_experience, application_not_found) {
    helper_insert_application("William", "Sugden");
    /* search for a different name */
    cr_assert(eq(int, addExperience("Tanay", "Kapoor", "Google", 2018, 2022, MANAGER), FAILURE), "should return FAILURE when no application matches first+last name");
}

Test(test_add_experience, last_name_mismatch) {
    helper_insert_application("Tanay", "Kapoor");
    /* correct first name but wrong last name */
    cr_assert(eq(int, addExperience("Tanay", "Wrong", "Google", 2018, 2022, MANAGER), FAILURE), "should return FAILURE when last name does not match");
}

// Matching application found, experiences array full -> ERROR

Test(test_add_experience, experiences_array_full) {
    helper_insert_application("Tanay", "Kapoor");
    /* fill all MAX_EXPER_LEN slots directly */
    for (int j = 0; j < MAX_EXPER_LEN; j++) {
        strcpy(applications[0].experiences[j].company_name, "Old_comapny");
        applications[0].experiences[j].start_year = 2000 + j;
        applications[0].experiences[j].end_year = 2001 + j;
        applications[0].experiences[j].experience_type = INTERN;
    }
    cr_assert(eq(int, addExperience("Tanay", "Kapoor", "Palantir", 2010, 2012, MANAGER), ERROR), "should return ERROR when the experiences array is full");
}

//  SUCCESS cases

Test(test_add_experience, add_experience_returns_success) {
    helper_insert_application("Tanay", "Kapoor");
    cr_assert(eq(int, addExperience("Tanay", "Kapoor", "Google", 2018, 2022, MANAGER), SUCCESS), "valid addExperience should return SUCCESS");
}

/* Verify all fields are written correctly after a successful add */
Test(test_add_experience, add_experience_fields_set_correctly) {
    helper_insert_application("Tanay", "Kapoor");
    addExperience("Tanay", "Kapoor", "Google", 2018, 2022, MANAGER);
    cr_assert(eq(str, applications[0].experiences[0].company_name, "Google"), "company_name should be set to 'Google'");
    cr_assert(eq(int, applications[0].experiences[0].start_year, 2018), "start_year should be 2018");
    cr_assert(eq(int, applications[0].experiences[0].end_year, 2022), "end_year should be 2022");
    cr_assert(eq(int, applications[0].experiences[0].experience_type, MANAGER), "experience_type should be MANAGER");
}

/* Add two experiences sequentially; second goes into slot [1] */
Test(test_add_experience, add_two_experiences) {
    helper_insert_application("Joel", "Johnson");
    cr_assert(eq(int, addExperience("Joel", "Johnson", "Startup", 2015, 2018, INTERN), SUCCESS), "first addExperience should return SUCCESS");
    cr_assert(eq(int, addExperience("Joel", "Johnson", "Google", 2019, 2022, ENGINEER_SENIOR), SUCCESS), "second addExperience should return SUCCESS");
    cr_assert(eq(str, applications[0].experiences[0].company_name, "Startup"), "first experience company_name should be 'Startup'");
    cr_assert(eq(str, applications[0].experiences[1].company_name, "Google"), "second experience company_name should be 'BigCorp'");
}

/* end_year == 2026 is valid (boundary) */
Test(test_add_experience, end_year_2026_is_valid) {
    helper_insert_application("Tanay", "Kapoor");
    cr_assert(eq(int, addExperience("Tanay", "Kapoor", "Google", 2020, 2026, CEO), SUCCESS), "end_year=2026 should be valid");
}

/* start_year == end_year is valid */
Test(test_add_experience, start_year_equals_end_year) {
    helper_insert_application("Tanay", "Kapoor");
    cr_assert(eq(int, addExperience("Tanay", "Kapoor", "Google", 2022, 2022, INTERN), SUCCESS), "start_year == end_year should be valid");
}
