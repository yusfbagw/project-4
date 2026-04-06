#include "../humanresources.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_find_duplicates, .timeout = UNREASONABLY_LONG);

Test(test_find_duplicates, no_applications) {
    app_count = 0;
    cr_assert(eq(int, findDuplicates(), FAILURE), "findDuplicates should return FAILURE when there are no applications");
}

Test(test_find_duplicates, no_duplicates) {
    app_count = 0;
    helper_addApplication("Kyryll", "Flins", 600, 5, 1);
    helper_addApplication("Kazuha", "Kaedehara", 21, 10, 0);
    helper_addApplication("Diluc", "Ragnvindr", 22, 15, 1);
    cr_assert(eq(int, findDuplicates(), FAILURE), "findDuplicates should return FAILURE when there are no duplicates");
}

Test(test_find_duplicates, different_first_names) {
    app_count = 0;
    helper_addApplication("Lyney", "Hearth", 20, 1, 0); // !
    helper_addApplication("Kyryll", "Flins", 600, 5, 1);
    helper_addApplication("Kazuha", "Kaedehara", 21, 10, 0);
    helper_addApplication("Lynette", "Hearth", 20, 1, 0); // !
    helper_addApplication("Diluc", "Ragnvindr", 22, 15, 1);
    cr_assert(eq(int, findDuplicates(), FAILURE), "findDuplicates should return FAILURE when first names are different");
}

Test(test_find_duplicates, different_last_names) {
    app_count = 0;
    helper_addApplication("Kyryll", "Flins", 600, 5, 1);
    helper_addApplication("Kamisato", "Ayaka", 20, 1, 0); // !
    helper_addApplication("Kazuha", "Kaedehara", 21, 10, 0);
    helper_addApplication("Kamisato", "Ayato", 20, 1, 0); // !
    helper_addApplication("Diluc", "Ragnvindr", 22, 15, 1);
    cr_assert(eq(int, findDuplicates(), FAILURE), "findDuplicates should return FAILURE when last names are different");
}

Test(test_find_duplicates, different_ages) {
    app_count = 0;
    helper_addApplication("Kyryll", "Flins", 600, 5, 1);
    helper_addApplication("Kusanali", "Buer", 5000, 100, 1); // ! (age 5000)
    helper_addApplication("Kazuha", "Kaedehara", 21, 10, 0);
    helper_addApplication("Diluc", "Ragnvindr", 22, 15, 1);
    helper_addApplication("Kusanali", "Buer", 500, 1, 1); // ! (age 500)
    cr_assert(eq(int, findDuplicates(), FAILURE), "findDuplicates should return FAILURE when ages are different");
}

Test(test_find_duplicates, different_projects) {
    app_count = 0;
    helper_addApplication("Kyryll", "Flins", 600, 5, 1);
    helper_addApplication("Kazuha", "Kaedehara", 21, 10, 0);
    helper_addApplication("Jason", "Tran", 20, 6, 0); // !
    helper_addApplication("Diluc", "Ragnvindr", 22, 15, 1);
    helper_addApplication("Jason", "Tran", 20, 7, 0); // !
    cr_assert(eq(int, findDuplicates(), SUCCESS), "findDuplicates should return SUCCESS when only project_count is different");
}

Test(test_find_duplicates, different_referral) {
    app_count = 0;
    helper_addApplication("Jason", "Tran", 20, 6, 0); // !
    helper_addApplication("Kyryll", "Flins", 600, 5, 1);
    helper_addApplication("Kazuha", "Kaedehara", 21, 10, 0);
    helper_addApplication("Diluc", "Ragnvindr", 22, 15, 1);
    helper_addApplication("Jason", "Tran", 20, 6, 1); // !
    cr_assert(eq(int, findDuplicates(), SUCCESS), "findDuplicates should return SUCCESS when only has_referral is different");
}
