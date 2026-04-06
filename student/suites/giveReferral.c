#include "../humanresources.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_give_referral, .timeout = UNREASONABLY_LONG);

Test(test_give_referral, empty_name) {
    app_count = 0;
    cr_assert(eq(int, ERROR, giveReferral(NULL, NULL)), "giveReferral should return ERROR when first_name and last_name are NULL");
    cr_assert(eq(int, ERROR, giveReferral("Jason", NULL)), "giveReferral should return ERROR when first_name is NULL");
    cr_assert(eq(int, ERROR, giveReferral(NULL, "Tran")), "giveReferral should return ERROR when last_name is NULL");
}

Test(test_give_referral, long_name) {
    char long_name[] = "This. Has 50 characters minus the null terminator.";
    app_count = 0;
    cr_assert(eq(int, ERROR, giveReferral(long_name, "Tran")), "giveReferral should return ERROR when first_name is too long");
    cr_assert(eq(int, ERROR, giveReferral("Jason", long_name)), "giveReferral should return ERROR when last_name is too long");
}

Test(test_give_referral, very_long_name) {
    char long_name[] = "This is a really long name, far surpassing the allowed length.";
    app_count = 0;
    cr_assert(eq(int, ERROR, giveReferral(long_name, "Tran")), "giveReferral should return ERROR when first_name is too long");
    cr_assert(eq(int, ERROR, giveReferral("Jason", long_name)), "giveReferral should return ERROR when last_name is too long");
}

Test(test_give_referral, missing_name_empty) {
    app_count = 0;
    cr_assert(eq(int, FAILURE, giveReferral("Jason", "Tran")), "giveReferral should return FAILURE when application is not found");
}

Test(test_give_referral, missing_name_populated) {
    app_count = 0;
    helper_addApplication("Kyryll", "Flins", 600, 5, 1);
    helper_addApplication("Kazuha", "Kaedehara", 21, 10, 0);
    helper_addApplication("Diluc", "Ragnvindr", 22, 15, 1);
    cr_assert(eq(int, FAILURE, giveReferral("Jason", "Tran")), "giveReferral should return FAILURE when application is not found");
}

Test(test_give_referral, success) {
    app_count = 0;
    helper_addApplication("Kyryll", "Flins", 600, 5, 1);
    helper_addApplication("Jason", "Tran", 20, 6, 0);
    helper_addApplication("Kazuha", "Kaedehara", 21, 10, 0);
    helper_addApplication("Diluc", "Ragnvindr", 22, 15, 1);

    struct Application applications_expected[4];
    memcpy(applications_expected, applications, sizeof(applications_expected));
    applications_expected[1].has_referral = 1;

    cr_assert(eq(int, SUCCESS, giveReferral("Jason", "Tran")), "giveReferral should return SUCCESS when applicant is found");
    for (int i = 0; i < 4; i++) {
        cr_assert(eq(int, applications[i].has_referral, applications_expected[i].has_referral), "giveReferral should update has_referral for the name provided and nothing else");
    }
}
