#include "../humanresources.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

TestSuite(test_make_decision, .timeout = UNREASONABLY_LONG);

/*
makeDecision validates min_rating and role itself (before delegating to
calculateRating / isOverqualified), so:
min_rating < 0 -> ERROR
role out of [INTERN, CEO] -> ERROR
All other app validation is delegated: if calculateRating or isOverqualified
returns ERROR, makeDecision returns ERROR.
*/

// ERROR cases

Test(test_make_decision, null_app) {
    cr_assert(eq(int, makeDecision(NULL, 10, MANAGER), ERROR), "NULL app should return ERROR");
}

Test(test_make_decision, invalid_app_empty_first_name) {
    struct Application app = {0};
    strcpy(app.last_name, "Kapoor");
    app.age = 25;
    app.project_count = 1;
    app.has_referral = 0;
    cr_assert(eq(int, makeDecision(&app, 10, MANAGER), ERROR), "invalid app (empty first_name) should propagate ERROR");
}

Test(test_make_decision, negative_min_rating) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 25;
    app.project_count = 1;
    app.has_referral = 0;
    cr_assert(eq(int, makeDecision(&app, -1, MANAGER), ERROR), "min_rating < 0 should return ERROR");
}

Test(test_make_decision, role_below_intern) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 25;
    app.project_count = 1;
    app.has_referral = 0;
    cr_assert(eq(int, makeDecision(&app, 10, (enum ExperienceType)(-1)), ERROR), "role below INTERN should return ERROR");
}

Test(test_make_decision, role_above_ceo) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 25;
    app.project_count = 1;
    app.has_referral = 0;
    cr_assert(eq(int, makeDecision(&app, 10, (enum ExperienceType)(CEO + 1)), ERROR), "role above CEO should return ERROR");
}

// FAILURE for when rating too low

/* rating = 0 + 1*5 + 0 = 5; min_rating = 10 -> rejected */
Test(test_make_decision, rating_below_minimum) {
    struct Application app = {0};
    strcpy(app.first_name, "Tanay");
    strcpy(app.last_name, "Kapoor");
    app.age = 25;
    app.project_count = 1; /* +5 */
    app.has_referral = 0;
    /* rating = 5; min_rating = 10 -> FAILURE */
    cr_assert(eq(int, makeDecision(&app, 10, MANAGER), FAILURE), "rating (5) below min_rating (10) should return FAILURE");
}

// Boundary check rating == min_rating - 1 (just one short) -> FAILURE
Test(test_make_decision, rating_one_below_minimum) {
    struct Application app = {0};
    strcpy(app.first_name, "William");
    strcpy(app.last_name, "Sugden");
    app.age = 25;
    app.project_count = 3; /* +15 */
    app.has_referral = 0;
    /* rating = 15; min_rating = 16 -> FAILURE */
    cr_assert(eq(int, makeDecision(&app, 16, MANAGER), FAILURE), "rating (15) one below min_rating (16) should return FAILURE");
}

// FAILURE when overqualified

/* rating >= min_rating but most recent experience_type > role -> rejected */
Test(test_make_decision, overqualified_rejected) {
    struct Application app = {0};
    strcpy(app.first_name, "Joel");
    strcpy(app.last_name, "Johnson");
    app.age = 40;
    app.project_count = 3; /* +15 */
    app.has_referral = 1;  /* +10 */
    strcpy(app.experiences[0].company_name, "BigTech");
    app.experiences[0].experience_type = CEO;
    app.experiences[0].experience_type = 5;
    app.experiences[0].start_year = 2010;
    app.experiences[0].end_year = 2022;
    /* rating = 5*12 + 15 + 10 = 85; min_rating = 10; but CEO > INTERN -> FAILURE */
    cr_assert(eq(int, makeDecision(&app, 10, INTERN), FAILURE), "overqualified applicant (CEO for INTERN) should return FAILURE even with high rating");
}

// SUCCESS cases

/* rating >= min_rating and not overqualified -> accepted */
Test(test_make_decision, accepted_meets_minimum) {
    struct Application app = {0};
    strcpy(app.first_name, "Michael");
    strcpy(app.last_name, "Yi");
    app.age = 28;
    app.project_count = 3; /* +15 */
    app.has_referral = 1;  /* +10 */
    /* no experiences, rating = 0 + 15 + 10 = 25; min_rating = 25; not overqualified */
    cr_assert(eq(int, makeDecision(&app, 25, MANAGER), SUCCESS), "rating (25) == min_rating (25) and not overqualified should return SUCCESS");
}

/* rating well above minimum, not overqualified -> accepted */
Test(test_make_decision, accepted_rating_above_minimum) {
    struct Application app = {0};
    strcpy(app.first_name, "Rohit");
    strcpy(app.last_name, "Rao");
    app.age = 30;
    app.project_count = 5; /* +25 */
    app.has_referral = 1;  /* +10 */
    strcpy(app.experiences[0].company_name, "StartupCo");
    app.experiences[0].experience_type = 2;
    app.experiences[0].start_year = 2018;
    app.experiences[0].end_year = 2022;
    /* exp_value = 2*4 = 8; rating = 8 + 25 + 10 = 43; min_rating = 20 */
    /* ENGINEER_JUNIOR not overqualified for MANAGER */
    cr_assert(eq(int, makeDecision(&app, 20, MANAGER), SUCCESS), "rating (43) above min_rating (20) and not overqualified should return SUCCESS");
}

/* min_rating = 0: any valid non-negative rating passes if not overqualified */
Test(test_make_decision, min_rating_zero_accepted) {
    struct Application app = {0};
    strcpy(app.first_name, "Navin");
    strcpy(app.last_name, "Senthil");
    app.age = 22;
    app.project_count = 1; /* +5 */
    app.has_referral = 0;
    /* rating = 5; min_rating = 0; not overqualified -> SUCCESS */
    cr_assert(eq(int, makeDecision(&app, 0, MANAGER), SUCCESS), "min_rating=0 should accept any applicant with non-negative rating who is not overqualified");
}
