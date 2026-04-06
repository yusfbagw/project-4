#include "../humanresources.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

static void setup(void) {
    helper_reset_applications();
}

TestSuite(test_sort_by_rating, .timeout = UNREASONABLY_LONG, .init = setup);

Test(test_sort_by_rating, empty_array) {
    int ret = sortByRating();
    cr_assert(eq(int, ret, SUCCESS),
        "sortByRating on empty array should return SUCCESS, got %d", ret);
}

Test(test_sort_by_rating, single_application) {
    helper_add_app("John", "Doe", 25, 5, 0);

    int ret = sortByRating();
    cr_assert(eq(int, ret, SUCCESS),
        "sortByRating with single app should return SUCCESS, got %d", ret);
    cr_assert(eq(str, applications[0].first_name, "John"),
        "Single application should remain in place");
}

Test(test_sort_by_rating, two_apps_higher_first) {
    helper_add_app("Low", "Rating", 25, 1, 0);
    helper_add_app("High", "Rating", 30, 5, 1);
    helper_add_exp(1, "Google", 2018, 2023, ENGINEER_SENIOR);

    int ret = sortByRating();
    cr_assert(eq(int, ret, SUCCESS),
        "sortByRating should return SUCCESS, got %d", ret);
    cr_assert(eq(str, applications[0].first_name, "High"),
        "Higher rated app should be first after sort");
    cr_assert(eq(str, applications[1].first_name, "Low"),
        "Lower rated app should be second after sort");
}

Test(test_sort_by_rating, same_rating_tiebreak_by_projects) {
    helper_add_app("Less", "Projects", 25, 1, 1);
    helper_add_app("More", "Projects", 30, 3, 0);

    int ret = sortByRating();
    cr_assert(eq(int, ret, SUCCESS),
        "sortByRating should return SUCCESS, got %d", ret);
    cr_assert(eq(str, applications[0].first_name, "More"),
        "App with more projects should be first when ratings are tied");
    cr_assert(eq(str, applications[1].first_name, "Less"),
        "App with fewer projects should be second when ratings are tied");
}

Test(test_sort_by_rating, multiple_applications) {
    helper_add_app("Zero", "Rating", 25, 0, 0);
    helper_add_app("Mid", "Rating", 30, 2, 0);
    helper_add_app("High", "Rating", 35, 5, 1);
    helper_add_exp(2, "Google", 2018, 2023, MANAGER);

    int ret = sortByRating();
    cr_assert(eq(int, ret, SUCCESS),
        "sortByRating should return SUCCESS, got %d", ret);
    cr_assert(eq(str, applications[0].first_name, "High"),
        "Highest rated app should be first");
    cr_assert(eq(str, applications[1].first_name, "Mid"),
        "Middle rated app should be second");
    cr_assert(eq(str, applications[2].first_name, "Zero"),
        "Lowest rated app should be third");
}

Test(test_sort_by_rating, already_sorted) {
    helper_add_app("High", "Rating", 35, 5, 1);
    helper_add_exp(0, "Google", 2018, 2023, MANAGER);

    helper_add_app("Mid", "Rating", 30, 2, 0);
    helper_add_app("Low", "Rating", 25, 0, 0);

    int ret = sortByRating();
    cr_assert(eq(int, ret, SUCCESS),
        "sortByRating should return SUCCESS, got %d", ret);
    cr_assert(eq(str, applications[0].first_name, "High"),
        "Already sorted: highest should remain first");
    cr_assert(eq(str, applications[1].first_name, "Mid"),
        "Already sorted: middle should remain second");
    cr_assert(eq(str, applications[2].first_name, "Low"),
        "Already sorted: lowest should remain third");
}

Test(test_sort_by_rating, reverse_sorted) {
    helper_add_app("Low", "Rating", 25, 0, 0);
    helper_add_app("Mid", "Rating", 30, 2, 0);
    helper_add_app("High", "Rating", 35, 5, 1);
    helper_add_exp(2, "Google", 2018, 2023, MANAGER);

    int ret = sortByRating();
    cr_assert(eq(int, ret, SUCCESS),
        "sortByRating should return SUCCESS, got %d", ret);
    cr_assert(eq(str, applications[0].first_name, "High"),
        "Reverse sorted: highest should be first after sort");
    cr_assert(eq(str, applications[1].first_name, "Mid"),
        "Reverse sorted: middle should be second after sort");
    cr_assert(eq(str, applications[2].first_name, "Low"),
        "Reverse sorted: lowest should be third after sort");
}

Test(test_sort_by_rating, verify_full_app_preserved) {
    helper_add_app("Low", "Rating", 20, 1, 0);
    helper_add_app("High", "Rating", 35, 5, 1);
    helper_add_exp(1, "Google", 2020, 2023, ENGINEER_JUNIOR);

    sortByRating();

    cr_assert(eq(str, applications[0].first_name, "High"),
        "Higher rated app should be first");
    cr_assert(eq(int, applications[0].age, 35),
        "Age should be preserved after sort");
    cr_assert(eq(int, applications[0].project_count, 5),
        "Project count should be preserved after sort");
    cr_assert(eq(int, applications[0].has_referral, 1),
        "Referral status should be preserved after sort");
    cr_assert(eq(str, applications[0].experiences[0].company_name, "Google"),
        "Experience data should be preserved after sort");
}

Test(test_sort_by_rating, four_apps_mixed) {
    helper_add_app("AppA", "Test", 25, 4, 0);
    helper_add_app("AppB", "Test", 26, 1, 1);
    helper_add_app("AppC", "Test", 27, 3, 0);
    helper_add_exp(2, "Startup", 2020, 2022, INTERN);
    helper_add_app("AppD", "Test", 28, 6, 1);
    helper_add_exp(3, "BigCorp", 2018, 2022, ENGINEER_JUNIOR);

    sortByRating();

    cr_assert(eq(str, applications[0].first_name, "AppD"),
        "AppD (rating 48) should be first");
    cr_assert(eq(str, applications[1].first_name, "AppA"),
        "AppA (rating 20) should be second");
    cr_assert(eq(str, applications[2].first_name, "AppC"),
        "AppC (rating 17) should be third");
    cr_assert(eq(str, applications[3].first_name, "AppB"),
        "AppB (rating 15) should be fourth");
}
