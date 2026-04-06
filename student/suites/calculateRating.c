#include "../humanresources.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

TestSuite(test_calculate_rating, .timeout = UNREASONABLY_LONG);

/*
Preconditions that must be checked and tested for not on the PDF):
app->first_name must be non-empty, otherwise ERROR is returned
app->last_name must be non-empty, otherwise ERROR is returned
app->age must be >= 0, otherwise ERROR is returned
app->project_count must be >= 0, otherwise ERROR is returned
app->has_referral must be 0 or 1, otherwise ERROR is returned
*/

// NULL/bad arg checks

Test(test_calculate_rating, null_app) {
    cr_assert(eq(int, calculateRating(NULL), ERROR), "calculateRating(NULL) should return ERROR");
}

Test(test_calculate_rating, empty_first_name) {
    struct Application app = {0};
    strcpy(app.first_name, "");
    strcpy(app.last_name, "Kapoor");
    app.age = 25;
    app.project_count = 0;
    app.has_referral = 0;
    cr_assert(eq(int, calculateRating(&app), ERROR), "calculateRating with empty first_name should return ERROR");
}

Test(test_calculate_rating, empty_last_name) {
    struct Application app = {0};
    strcpy(app.first_name, "jj_jjjj");
    strcpy(app.last_name, "");
    app.age = 25;
    app.project_count = 0;
    app.has_referral = 0;
    cr_assert(eq(int, calculateRating(&app), ERROR), "calculateRating with empty last_name should return ERROR");
}

Test(test_calculate_rating, negative_age) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = -1;
    app.project_count = 0;
    app.has_referral = 0;
    cr_assert(eq(int, calculateRating(&app), ERROR), "calculateRating with negative age should return ERROR");
}

Test(test_calculate_rating, negative_project_count) {
    struct Application app = {0};
    strcpy(app.first_name, "William");
    strcpy(app.last_name, "Sugden");
    app.age = 30;
    app.project_count = -1;
    app.has_referral = 0;
    cr_assert(eq(int, calculateRating(&app), ERROR), "calculateRating with negative project_count should return ERROR");
}

Test(test_calculate_rating, invalid_referral_above_one) {
    struct Application app = {0};
    strcpy(app.first_name, "Joel");
    strcpy(app.last_name, "Johnson");
    app.age = 30;
    app.project_count = 1;
    app.has_referral = 2;
    cr_assert(eq(int, calculateRating(&app), ERROR), "calculateRating with has_referral=2 should return ERROR");
}

Test(test_calculate_rating, invalid_referral_negative) {
    struct Application app = {0};
    strcpy(app.first_name, "Michael");
    strcpy(app.last_name, "Yi");
    app.age = 30;
    app.project_count = 1;
    app.has_referral = -1;
    cr_assert(eq(int, calculateRating(&app), ERROR), "calculateRating with has_referral=-1 should return ERROR");
}

// negative rating should return 0 (ERROR), they must clamp to 0

/* No experiences, 0 projects (-5), no referral: rating = -5 -> clamped to 0 */
Test(test_calculate_rating, zero_projects_no_referral_clamped) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 22;
    app.project_count = 0;
    app.has_referral = 0;
    cr_assert(eq(int, calculateRating(&app), 0), "rating of -5 (0 projects, no referral, no exp) should be clamped to 0");
}

/* Single low-weight experience + 0 projects still produces negative result and should return 0
   exp[0]: experience_type=1, years=2021-2020=1 -> exp_value=1
   project_val=-5, referral_val=0 -> rating = 1-5 = -4 -> 0 */
Test(test_calculate_rating, negative_rating_clamped_to_zero) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 24;
    strcpy(app.experiences[0].company_name, "Startup");
    app.experiences[0].experience_type = 1;
    app.experiences[0].start_year = 2020;
    app.experiences[0].end_year = 2021;
    app.project_count = 0;
    app.has_referral = 0;
    cr_assert(eq(int, calculateRating(&app), 0), "rating = 1 (exp) - 5 (0 projects) = -4 should be clamped to 0");
}

// project values are calcualted correctly

/* No experiences, project_count=4, no referral: 4*5 = 20 */
Test(test_calculate_rating, no_experience_with_projects) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 28;
    app.project_count = 4;
    app.has_referral = 0;
    cr_assert(eq(int, calculateRating(&app), 20), "rating with 4 projects and no experience should be 20");
}

/* No experiences, 0 projects (-5), has_referral=1 (+10): -5+10 = 5 */
Test(test_calculate_rating, zero_projects_with_referral) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 21;
    app.project_count = 0;
    app.has_referral = 1;
    cr_assert(eq(int, calculateRating(&app), 5), "rating with 0 projects (-5) and referral (+10) should be 5");
}

// test ratings with different experience values

/* Single experience: experience_type=2, years=2020-2015=5 -> exp_value=10
   project_count=1 (+5); no referral -> 15 */
Test(test_calculate_rating, single_experience_basic) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 30;
    strcpy(app.experiences[0].company_name, "Meta");
    app.experiences[0].experience_type = 2;
    app.experiences[0].start_year = 2015;
    app.experiences[0].end_year = 2020;
    app.project_count = 1;
    app.has_referral = 0;
    /* exp_value = 2*5 = 10; project_val = 5 -> 15 */
    cr_assert(eq(int, calculateRating(&app), 15), "single experience (pos=2, years=5) + 1 project should give rating 15");
}

/* Years worked = 0 (start_year == end_year) must be treated as 1 year:
   experience_type=3, years=0->1 -> exp_value=3; project_count=1 (+5); no referral -> 8 */
Test(test_calculate_rating, years_worked_zero_treated_as_one) {
    struct Application app = {0};
    strcpy(app.first_name, "Christopher");
    strcpy(app.last_name, "Goggins");
    app.age = 26;
    strcpy(app.experiences[0].company_name, "Google");
    app.experiences[0].experience_type = 3;
    app.experiences[0].start_year = 2022;
    app.experiences[0].end_year = 2022;
    app.project_count = 1;
    app.has_referral = 0;
    /* exp_value = 3*1 = 3; project_val = 5 -> 8 */
    cr_assert(eq(int, calculateRating(&app), 8), "experience with start==end year should count as 1 year worked; rating = 3+5 = 8");
}

/* Multiple experiences are all summed:
   exp[0]: pos=1, years=2013-2010=3 -> 3
   exp[1]: pos=4, years=2017-2014=3 -> 12
   exp[2]: pos=2, years=2020-2018=2 -> 4
   exp_value=19; project_count=2 (+10); has_referral=1 (+10) -> 39 */
Test(test_calculate_rating, multiple_experiences_summed) {
    struct Application app = {0};
    strcpy(app.first_name, "J");
    strcpy(app.last_name, "Tran");
    app.age = 35;
    strcpy(app.experiences[0].company_name, "A");
    app.experiences[0].experience_type = 1;
    app.experiences[0].start_year = 2010;
    app.experiences[0].end_year = 2013;
    strcpy(app.experiences[1].company_name, "B");
    app.experiences[1].experience_type = 4;
    app.experiences[1].start_year = 2014;
    app.experiences[1].end_year = 2017;
    strcpy(app.experiences[2].company_name, "C");
    app.experiences[2].experience_type = 2;
    app.experiences[2].start_year = 2018;
    app.experiences[2].end_year = 2020;
    app.project_count = 2;
    app.has_referral = 1;
    cr_assert(eq(int, calculateRating(&app), 39), "multiple experiences (3+12+4=19) + 2 projects (+10) + referral (+10) should be 39");
}

/* Zeroed experience entries (company_name[0]=='\0' and experience_type==0) must not be counted:
   exp[0]: pos=5, years=2020-2018=2 -> exp_value=10; rest zeroed
   project_count=1 (+5); no referral -> 15 */
Test(test_calculate_rating, zeroed_experience_entries_ignored) {
    struct Application app = {0};
    strcpy(app.first_name, "Rohit");
    strcpy(app.last_name, "Rao");
    app.age = 29;
    strcpy(app.experiences[0].company_name, "Fortnite");
    app.experiences[0].experience_type = 5;
    app.experiences[0].start_year = 2018;
    app.experiences[0].end_year = 2020;
    /* experiences[1..9] remain zeroed and should not be counted */
    app.project_count = 1;
    app.has_referral = 0;
    /* exp_value = 5*2 = 10; project_val = 5 -> 15 */
    cr_assert(eq(int, calculateRating(&app), 15), "zeroed experience entries should not contribute to exp_value; rating = 10+5 = 15");
}

// test different referral values

/* No experiences, project_count=2 (+10), has_referral=1 (+10): 20 */
Test(test_calculate_rating, referral_adds_ten_points) {
    struct Application app = {0};
    strcpy(app.first_name, "Suraj");
    strcpy(app.last_name, "Sumadrula");
    app.age = 27;
    app.project_count = 2;
    app.has_referral = 1;
    cr_assert(eq(int, calculateRating(&app), 20), "referral (+10) + 2 projects (+10) should give rating 20");
}

/* --- Full combination test --- */

/* exp[0]: pos=5, years=2020-2010=10 -> 50
   exp[1]: pos=3, years=2008-2005=3  -> 9
   exp_value=59; project_count=3 (+15); has_referral=1 (+10) -> 84 */
Test(test_calculate_rating, full_calculation_all_components) {
    struct Application app = {0};
    strcpy(app.first_name, "Navin");
    strcpy(app.last_name, "Senthil");
    app.age = 40;
    strcpy(app.experiences[0].company_name, "UPS");
    app.experiences[0].experience_type = 5;
    app.experiences[0].start_year = 2010;
    app.experiences[0].end_year = 2020;
    strcpy(app.experiences[1].company_name, "Tesla");
    app.experiences[1].experience_type = 3;
    app.experiences[1].start_year = 2005;
    app.experiences[1].end_year = 2008;
    app.project_count = 3;
    app.has_referral = 1;
    /* exp_value = 50+9 = 59; project_val = 15; referral_val = 10 -> 84 */
    cr_assert(eq(int, calculateRating(&app), 84), "full calculation: exp(50+9=59) + projects(15) + referral(10) = 84");
}

/* Edge case: age=0 is valid (boundary of age >= 0)
   No experiences, project_count=1 (+5), no referral -> 5 */
Test(test_calculate_rating, age_zero_is_valid) {
    struct Application app = {0};
    strcpy(app.first_name, "Daniel");
    strcpy(app.last_name, "Huang");
    app.age = 0;
    app.project_count = 1;
    app.has_referral = 0;
    cr_assert(eq(int, calculateRating(&app), 5), "age=0 should be valid; rating = 0 (exp) + 5 (1 project) + 0 = 5");
}
