#include "../humanresources.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_mass_layoffs, .timeout = UNREASONABLY_LONG);

Test(test_mass_layoffs, no_applications) {
    app_count = 0;
    cr_assert(eq(int, SUCCESS, massLayoffs()), "massLayoffs should always return SUCCESS");
}

Test(test_mass_layoffs, no_layoffs) {
    app_count = 0;
    helper_addApplication("Kamisato", "Ayaka", 20, 10, 0);
    helper_addApplication("Kyryll", "Flins", 600, 5, 1);
    helper_addExperience("Kyryll", "Flins", "Lightkeepers", 1500, 2026, ENGINEER_SENIOR);
    helper_addApplication("Kazuha", "Kaedehara", 21, 3, 0);
    helper_addApplication("Ajax", "Tartaglia", 22, 4, 1);
    helper_addApplication("Kamisato", "Ayato", 20, 3, 0);
    helper_addExperience("Kamisato", "Ayato", "Yashiro", 1998, 2026, CEO);

    int expected_app_count = app_count;
    struct Application expected_applications[5];
    memcpy(expected_applications, applications, sizeof(expected_applications));

    cr_assert(eq(int, SUCCESS, massLayoffs()), "massLayoffs should always return SUCCESS");
    cr_assert(eq(int, app_count, expected_app_count), "massLayoffs should not change app_count when no applications are removable");
    cr_assert(memcmp(expected_applications, applications, sizeof(expected_applications)) == 0,
        "massLayoffs should not change applications when no applications are removable");
}

Test(test_mass_layoffs, remove_few_projects) {
    app_count = 0;
    helper_addApplication("Kamisato", "Ayaka", 20, 10, 0);
    helper_addApplication("Kyryll", "Flins", 600, 0, 1);
    helper_addExperience("Kyryll", "Flins", "Lightkeepers", 1500, 2026, ENGINEER_SENIOR);
    helper_addApplication("Kazuha", "Kaedehara", 21, 3, 0);
    helper_addApplication("Ajax", "Tartaglia", 22, 4, 1);
    helper_addApplication("Kamisato", "Ayato", 20, 3, 0);
    helper_addExperience("Kamisato", "Ayato", "Yashiro", 1998, 2026, CEO);
    
    int expected_app_count = 4;
    struct Application applications_copy[5];
    memcpy(applications_copy, applications, sizeof(applications_copy));
    // removal does swap
    int expected_order[] = {0, 4, 2, 3};

    cr_assert(eq(int, SUCCESS, massLayoffs()), "massLayoffs should always return SUCCESS");
    cr_assert(eq(int, app_count, expected_app_count), "massLayoffs should remove applications with fewer than 3 projects and update app_count accordingly");
    for (int i = 0; i < app_count; i++) {
        cr_assert(strncmp(applications[i].first_name, applications_copy[expected_order[i]].first_name, MAX_APPLICANT_NAME_LEN) == 0 &&
            strncmp(applications[i].last_name, applications_copy[expected_order[i]].last_name, MAX_APPLICANT_NAME_LEN) == 0 &&
            memcmp(applications[i].experiences, applications_copy[expected_order[i]].experiences, sizeof(struct Experience) * MAX_EXPER_LEN) == 0 &&
            applications[i].age == applications_copy[expected_order[i]].age &&
            applications[i].project_count == applications_copy[expected_order[i]].project_count &&
            applications[i].has_referral == applications_copy[expected_order[i]].has_referral,
            "massLayoffs should remove applications with fewer than 3 projects and keep the order of remaining applications consistent with swapping removed applications to the end of the array");
    }
}

Test(test_mass_layoffs, remove_few_projects_consecutive) {
    app_count = 0;
    helper_addApplication("Kamisato", "Ayaka", 20, 10, 0);
    helper_addApplication("Kyryll", "Flins", 600, 0, 1);
    helper_addExperience("Kyryll", "Flins", "Lightkeepers", 1500, 2026, ENGINEER_SENIOR);
    helper_addApplication("Kazuha", "Kaedehara", 21, 0, 0);
    helper_addApplication("Ajax", "Tartaglia", 22, 4, 1);
    helper_addApplication("Kamisato", "Ayato", 20, 3, 0);
    helper_addExperience("Kamisato", "Ayato", "Yashiro", 1998, 2026, CEO);
    
    int expected_app_count = 3;
    struct Application applications_copy[5];
    memcpy(applications_copy, applications, sizeof(applications_copy));
    // removal does swap
    int expected_order[] = {0, 3, 4};

    cr_assert(eq(int, SUCCESS, massLayoffs()), "massLayoffs should always return SUCCESS");
    cr_assert(eq(int, app_count, expected_app_count), "massLayoffs should remove applications with fewer than 3 projects and update app_count accordingly");
    for (int i = 0; i < app_count; i++) {
        cr_assert(strncmp(applications[i].first_name, applications_copy[expected_order[i]].first_name, MAX_APPLICANT_NAME_LEN) == 0 &&
            strncmp(applications[i].last_name, applications_copy[expected_order[i]].last_name, MAX_APPLICANT_NAME_LEN) == 0 &&
            memcmp(applications[i].experiences, applications_copy[expected_order[i]].experiences, sizeof(struct Experience) * MAX_EXPER_LEN) == 0 &&
            applications[i].age == applications_copy[expected_order[i]].age &&
            applications[i].project_count == applications_copy[expected_order[i]].project_count &&
            applications[i].has_referral == applications_copy[expected_order[i]].has_referral,
            "massLayoffs should remove applications with fewer than 3 projects and keep the order of remaining applications consistent with swapping removed applications to the end of the array");
    }
}

Test(test_mass_layoffs, remove_interns) {
    app_count = 0;
    helper_addApplication("Kamisato", "Ayaka", 20, 10, 0);
    helper_addApplication("Kyryll", "Flins", 600, 5, 1);
    helper_addExperience("Kyryll", "Flins", "Lightkeepers", 1500, 2020, ENGINEER_SENIOR);
    // this is more recent
    helper_addExperience("Kyryll", "Flins", "Lightkeepers", 2020, 2026, INTERN);
    helper_addApplication("Kazuha", "Kaedehara", 21, 3, 0);
    helper_addApplication("Ajax", "Tartaglia", 22, 4, 1);
    helper_addApplication("Kamisato", "Ayato", 20, 3, 0);
    helper_addExperience("Kamisato", "Ayato", "Yashiro", 1998, 2026, CEO);
    
    int expected_app_count = 4;
    struct Application applications_copy[5];
    memcpy(applications_copy, applications, sizeof(applications_copy));
    // removal does swap
    int expected_order[] = {0, 4, 2, 3};

    cr_assert(eq(int, SUCCESS, massLayoffs()), "massLayoffs should always return SUCCESS");
    cr_assert(eq(int, app_count, expected_app_count), "massLayoffs should remove INTERNs and update app_count accordingly");
    for (int i = 0; i < app_count; i++) {
        cr_assert(strncmp(applications[i].first_name, applications_copy[expected_order[i]].first_name, MAX_APPLICANT_NAME_LEN) == 0 &&
            strncmp(applications[i].last_name, applications_copy[expected_order[i]].last_name, MAX_APPLICANT_NAME_LEN) == 0 &&
            memcmp(applications[i].experiences, applications_copy[expected_order[i]].experiences, sizeof(struct Experience) * MAX_EXPER_LEN) == 0 &&
            applications[i].age == applications_copy[expected_order[i]].age &&
            applications[i].project_count == applications_copy[expected_order[i]].project_count &&
            applications[i].has_referral == applications_copy[expected_order[i]].has_referral,
            "massLayoffs should remove INTERNs and keep the order of remaining applications consistent with swapping removed applications to the end of the array");
    }
}

Test(test_mass_layoffs, remove_jt) {
    app_count = 0;
    helper_addApplication("Kamisato", "Ayaka", 20, 10, 0);
    helper_addApplication("Kyryll", "Flins", 600, 5, 1);
    helper_addExperience("Kyryll", "Flins", "Lightkeepers", 1500, 2026, ENGINEER_SENIOR);
    helper_addApplication("Kazuha", "Kaedehara", 21, 3, 0);
    helper_addApplication("Tartaglia", "Ajax", 22, 4, 1);
    helper_addApplication("Jean", "Gunnhildr", 20, 3, 0);
    
    int expected_app_count = 3;
    struct Application applications_copy[5];
    memcpy(applications_copy, applications, sizeof(applications_copy));
    // removal does swap
    int expected_order[] = {0, 1, 2};

    cr_assert(eq(int, SUCCESS, massLayoffs()), "massLayoffs should always return SUCCESS");
    cr_assert(eq(int, app_count, expected_app_count), "massLayoffs should remove INTERNs and update app_count accordingly");
    for (int i = 0; i < app_count; i++) {
        cr_assert(strncmp(applications[i].first_name, applications_copy[expected_order[i]].first_name, MAX_APPLICANT_NAME_LEN) == 0 &&
            strncmp(applications[i].last_name, applications_copy[expected_order[i]].last_name, MAX_APPLICANT_NAME_LEN) == 0 &&
            memcmp(applications[i].experiences, applications_copy[expected_order[i]].experiences, sizeof(struct Experience) * MAX_EXPER_LEN) == 0 &&
            applications[i].age == applications_copy[expected_order[i]].age &&
            applications[i].project_count == applications_copy[expected_order[i]].project_count &&
            applications[i].has_referral == applications_copy[expected_order[i]].has_referral,
            "massLayoffs should remove INTERNs and keep the order of remaining applications consistent with swapping removed applications to the end of the array");
    }
}
