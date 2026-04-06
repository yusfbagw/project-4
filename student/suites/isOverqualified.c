#include "../humanresources.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

TestSuite(Test_is_overqualified, .timeout = UNREASONABLY_LONG);

/*
Precondition checks not in PDF:
app->first_name must be non-empty, otherwise ERROR is returned
app->last_name must be non-empty, otherwise ERROR is returned
app->age must be >= 0, otherwise ERROR is returned
app->project_count must be >= 0, otherwise ERROR is returned
app->has_referral must be 0 or 1, otherwise ERROR is returned
role must be in range [INTERN, CEO] (i.e., a valid ExperienceType), otherwise ERROR is returned

Note: an experience entry is only considered if company_name is non-empty.
Note: the solution determines "most recent" using end_year (PDF says start_year).
All multi-experience Tests below are designed so start_year and end_year ordering agree.
*/

// NULL/invalid input Tests

Test(test_is_overqualified, null_app) {
    cr_assert(eq(int, isOverqualified(NULL, MANAGER), ERROR), "isOverqualified(NULL, ...) should return ERROR");
}

Test(test_is_overqualified, empty_first_name) {
    struct Application app = {0};
    strcpy(app.last_name, "Kapoor");
    app.age = 25;
    app.project_count = 0;
    app.has_referral = 0;
    cr_assert(eq(int, isOverqualified(&app, MANAGER), ERROR), "empty first_name should return ERROR");
}

Test(test_is_overqualified, empty_last_name) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    app.age = 25;
    app.project_count = 0;
    app.has_referral = 0;
    cr_assert(eq(int, isOverqualified(&app, MANAGER), ERROR), "empty last_name should return ERROR");
}

Test(test_is_overqualified, negative_age) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = -1;
    app.project_count = 0;
    app.has_referral = 0;
    cr_assert(eq(int, isOverqualified(&app, MANAGER), ERROR), "negative age should return ERROR");
}

Test(test_is_overqualified, negative_project_count) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 30;
    app.project_count = -1;
    app.has_referral = 0;
    cr_assert(eq(int, isOverqualified(&app, MANAGER), ERROR), "negative project_count should return ERROR");
}

Test(test_is_overqualified, invalid_referral_above_one) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 30;
    app.project_count = 0;
    app.has_referral = 2;
    cr_assert(eq(int, isOverqualified(&app, MANAGER), ERROR), "has_referral=2 should return ERROR");
}

Test(test_is_overqualified, invalid_referral_negative) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 30;
    app.project_count = 0;
    app.has_referral = -1;
    cr_assert(eq(int, isOverqualified(&app, MANAGER), ERROR), "has_referral=-1 should return ERROR");
}

Test(test_is_overqualified, role_below_intern) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 25;
    app.project_count = 0;
    app.has_referral = 0;
    cr_assert(eq(int, isOverqualified(&app, (enum ExperienceType)(-1)), ERROR), "role below INTERN should return ERROR");
}

Test(test_is_overqualified, role_above_ceo) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 25;
    app.project_count = 0;
    app.has_referral = 0;
    cr_assert(eq(int, isOverqualified(&app, (enum ExperienceType)(CEO + 1)), ERROR), "role above CEO should return ERROR");
}

// No experiences should default to Failure

Test(test_is_overqualified, no_experiences) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 22;
    app.project_count = 0;
    app.has_referral = 0;
    // All experiences zeroed (company_name[0]=='\0') -> none counted -> FAILURE
    cr_assert(eq(int, isOverqualified(&app, MANAGER), FAILURE), "applicant with no experiences should return FAILURE");
}

// test for single exp

/* experience_type > role -> SUCCESS */
Test(test_is_overqualified, single_experience_overqualified) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 35;
    app.project_count = 0;
    app.has_referral = 0;
    strcpy(app.experiences[0].company_name, "Palantir");
    app.experiences[0].experience_type = CEO;
    app.experiences[0].start_year = 2015;
    app.experiences[0].end_year = 2022;
    cr_assert(eq(int, isOverqualified(&app, MANAGER), SUCCESS), "CEO experience applying for MANAGER role should be overqualified");
}

/* experience_type == role -> FAILURE (not strictly greater) */
Test(test_is_overqualified, single_experience_equal_to_role) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 35;
    app.project_count = 0;
    app.has_referral = 0;
    strcpy(app.experiences[0].company_name, "Anduril");
    app.experiences[0].experience_type = MANAGER;
    app.experiences[0].start_year = 2015;
    app.experiences[0].end_year = 2022;
    cr_assert(eq(int, isOverqualified(&app, MANAGER), FAILURE), "MANAGER experience applying for MANAGER role should not be overqualified");
}

/* experience_type < role -> FAILURE */
Test(test_is_overqualified, single_experience_below_role) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 22;
    app.project_count = 0;
    app.has_referral = 0;
    strcpy(app.experiences[0].company_name, "Apple");
    app.experiences[0].experience_type = INTERN;
    app.experiences[0].start_year = 2021;
    app.experiences[0].end_year = 2022;
    cr_assert(eq(int, isOverqualified(&app, MANAGER), FAILURE), "INTERN experience applying for MANAGER role should not be overqualified");
}

// Multiple experiences only the most recent matters in the calculation

/* Older exp is high, newer exp is low -> NOT overqualified (most recent decides) */
Test(test_is_overqualified, most_recent_experience_not_overqualified) {
    struct Application app = {0};
    strcpy(app.first_name, "William");
    strcpy(app.last_name, "Sugden");
    app.age = 40;
    app.project_count = 0;
    app.has_referral = 0;
    /* Older experience: CEO -> would be overqualified */
    strcpy(app.experiences[0].company_name, "Nvidia");
    app.experiences[0].experience_type = CEO;
    app.experiences[0].start_year = 2010;
    app.experiences[0].end_year = 2015;
    /* Newer experience: INTERN -> not overqualified */
    strcpy(app.experiences[1].company_name, "Arm");
    app.experiences[1].experience_type = INTERN;
    app.experiences[1].start_year = 2018;
    app.experiences[1].end_year = 2022;
    cr_assert(eq(int, isOverqualified(&app, ENGINEER_SENIOR), FAILURE), "most recent experience (INTERN) is not overqualified for ENGINEER_SENIOR");
}

/* Older exp is low, newer exp is high -> overqualified (most recent decides) */
Test(test_is_overqualified, most_recent_experience_overqualified) {
    struct Application app = {0};
    strcpy(app.first_name, "Joel");
    strcpy(app.last_name, "Johnson");
    app.age = 40;
    app.project_count = 0;
    app.has_referral = 0;
    /* Older experience: INTERN -> would not be overqualified */
    strcpy(app.experiences[0].company_name, "Stealth");
    app.experiences[0].experience_type = INTERN;
    app.experiences[0].start_year = 2010;
    app.experiences[0].end_year = 2013;
    /* Newer experience: CEO -> overqualified */
    strcpy(app.experiences[1].company_name, "Google");
    app.experiences[1].experience_type = CEO;
    app.experiences[1].start_year = 2015;
    app.experiences[1].end_year = 2022;
    cr_assert(eq(int, isOverqualified(&app, MANAGER), SUCCESS), "most recent experience (CEO) is overqualified for MANAGER");
}

/* Experiences stored out of chronological order in the array:
   Array index 0 has a NEWER exp, array index 1 has an OLDER exp.
   The function must find the most recent by year, not by array position. */
Test(test_is_overqualified, experiences_out_of_array_order) {
    struct Application app = {0};
    strcpy(app.first_name, "Michael");
    strcpy(app.last_name, "Yi");
    app.age = 38;
    app.project_count = 0;
    app.has_referral = 0;
    /* Array index 0: chronologically NEWER, experience_type=INTERN (not overqualified) */
    strcpy(app.experiences[0].company_name, "NewPlace");
    app.experiences[0].experience_type = INTERN;
    app.experiences[0].start_year = 2018;
    app.experiences[0].end_year = 2022;
    /* Array index 1: chronologically OLDER, experience_type=CEO (would be overqualified) */
    strcpy(app.experiences[1].company_name, "OldPlace");
    app.experiences[1].experience_type = CEO;
    app.experiences[1].start_year = 2008;
    app.experiences[1].end_year = 2015;
    /* Most recent is exp[0] (INTERN), so NOT overqualified for MANAGER */
    cr_assert(eq(int, isOverqualified(&app, MANAGER), FAILURE), "most recent experience by year (INTERN) should decide, regardless of array order");
}

// Experience with empty company_name is ignored

Test(test_is_overqualified, empty_company_name_ignored) {
    struct Application app = {0};
    strcpy(app.first_name, "Rohit");
    strcpy(app.last_name, "Rao");
    app.age = 30;
    app.project_count = 0;
    app.has_referral = 0;
    /* experience_type=CEO but company_name is empty -> must be ignored */
    app.experiences[0].experience_type = CEO;
    app.experiences[0].start_year = 2015;
    app.experiences[0].end_year = 2022;
    /* company_name[0] remains '\0' */
    cr_assert(eq(int, isOverqualified(&app, INTERN), FAILURE), "experience with empty company_name should be ignored; no valid experiences -> FAILURE");
}

// Role boundary tests

/* Role = INTERN (lowest): any experience_type > INTERN is overqualified */
Test(test_is_overqualified, role_intern_overqualified_by_engineer) {
    struct Application app = {0};
    strcpy(app.first_name, "Suraj");
    strcpy(app.last_name, "Sumadrula");
    app.age = 25;
    app.project_count = 0;
    app.has_referral = 0;
    strcpy(app.experiences[0].company_name, "Company");
    app.experiences[0].experience_type = ENGINEER_JUNIOR;
    app.experiences[0].start_year = 2020;
    app.experiences[0].end_year = 2022;
    cr_assert(eq(int, isOverqualified(&app, INTERN), SUCCESS), "ENGINEER_JUNIOR experience is overqualified for INTERN role");
}

/* Role = CEO (highest): no experience_type can exceed CEO -> always FAILURE */
Test(Test_is_overqualified, role_ceo_nobody_overqualified) {
    struct Application app = {0};
    strcpy(app.first_name, "Navin");
    strcpy(app.last_name, "Senthil");
    app.age = 55;
    app.project_count = 0;
    app.has_referral = 0;
    strcpy(app.experiences[0].company_name, "Failing_Startup");
    app.experiences[0].experience_type = CEO;
    app.experiences[0].start_year = 2010;
    app.experiences[0].end_year = 2022;
    cr_assert(eq(int, isOverqualified(&app, CEO), FAILURE), "CEO experience applying for CEO role is not overqualified (not strictly greater)");
}
