#include "../humanresources.h"
#include "ag_utils.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

TestSuite(test_add_application, .timeout = UNREASONABLY_LONG);

/*
* This function adds an application to the array
* Returns SUCCESS if the application was added successfully
*
* You must copy the provided strings into the new struct and set the integer fields
* The new applicant's experiences array should be initialized as empty
* Returns ERROR if the array is full or the application cannot be added
*/

//invalid inputs
Test(test_add_application, invalid_inputs) {
    cr_assert_eq(addApplication(NULL, "Smith", 25, 3, 1), ERROR, "Expected ERROR when first name is NULL");
    cr_assert_eq(addApplication("John", NULL, 25, 3, 1), ERROR, "Expected ERROR when last name is NULL");
    cr_assert_eq(addApplication("", "Smith", 25, 3, 1), ERROR, "Expected ERROR when first name is empty");
    cr_assert_eq(addApplication("John", "", 25, 3, 1), ERROR, "Expected ERROR when last name is empty");
    cr_assert_eq(addApplication("John", "Smith", -1, 3, 1), ERROR, "Expected ERROR when age is negative");
    cr_assert_eq(addApplication("John", "Smith", 25, -1, 1), ERROR, "Expected ERROR when project count is negative");
    cr_assert_eq(addApplication("John", "Smith", 25, 3, -1), ERROR, "Expected ERROR when referral value is negative");
    cr_assert_eq(addApplication("John", "Smith", 25, 3, 2), ERROR, "Expected ERROR when referral value is greater than 1");
    char longName[MAX_APPLICANT_NAME_LEN + 1];
    memset(longName, 'A', MAX_APPLICANT_NAME_LEN);
    longName[MAX_APPLICANT_NAME_LEN] = '\0';
    cr_assert_eq(addApplication(longName, "Smith", 25, 3, 1), ERROR, "Expected ERROR when first name is too long");
    cr_assert_eq(addApplication("John", longName, 25, 3, 1), ERROR, "Expected ERROR when last name is too long");
}

//test adding a valid application and check if it is added correctly
Test(test_add_application, valid_application) {
    //reset applications array before test
    app_count = 0;
    memset(applications, 0, sizeof(applications));

    const char *first_name = "Alice";
    const char *last_name = "Johnson";
    int age = 30;
    int project_count = 5;
    int has_referral = 1;

    int result = addApplication(first_name, last_name, age, project_count, has_referral);
    cr_assert_eq(result, SUCCESS, "Expected SUCCESS when adding a valid application");

    // Check if the application was added correctly
    cr_assert_str_eq(applications[0].first_name, first_name, "Expected first name to be stored correctly; expected '%s', got '%s'", first_name, applications[0].first_name);
    cr_assert_str_eq(applications[0].last_name, last_name, "Expected last name to be stored correctly; expected '%s', got '%s'", last_name, applications[0].last_name);
    cr_assert_eq(applications[0].age, age, "Expected age to be stored correctly; expected %d, got %d", age, applications[0].age);
    cr_assert_eq(applications[0].project_count, project_count, "Expected project count to be stored correctly; expected %d, got %d", project_count, applications[0].project_count);
    cr_assert_eq(applications[0].has_referral, has_referral, "Expected referral value to be stored correctly; expected %d, got %d", has_referral, applications[0].has_referral);

    //check it experience array are initialized as empty and rights length
    for (int i = 0; i < MAX_EXPER_LEN; i++) {
        cr_assert_eq(applications[0].experiences[i].company_name[0], '\0', "Expected experience company name to be initialized as empty; expected '\\0', got '%c'", applications[0].experiences[i].company_name[0]);
        cr_assert_eq(applications[0].experiences[i].start_year, 0, "Expected experience start year to be initialized to 0; expected 0, got %d", applications[0].experiences[i].start_year);
        cr_assert_eq(applications[0].experiences[i].end_year, 0, "Expected experience end year to be initialized to 0; expected 0, got %d", applications[0].experiences[i].end_year);
        cr_assert_eq(applications[0].experiences[i].experience_type, INTERN, "Expected experience type to be initialized to INTERN; expected %d, got %d", INTERN, applications[0].experiences[i].experience_type);
    }

    //check if application count is updated
    cr_assert_eq(app_count, 1, "Expected application count to be updated to 1");

    //add another application and check if it is added correctly
    const char *first_name2 = "Bob";
    const char *last_name2 = "Smith";
    int age2 = 40;
    int project_count2 = 10;
    int has_referral2 = 0;
    
    result = addApplication(first_name2, last_name2, age2, project_count2, has_referral2);
    cr_assert_eq(result, SUCCESS, "Expected SUCCESS when adding a valid application; expected %d, got %d", SUCCESS, result);
    // Check if the second application was added correctly
    cr_assert_str_eq(applications[1].first_name, first_name2, "Expected second first name to be stored correctly; expected '%s', got '%s'", first_name2, applications[1].first_name);
    cr_assert_str_eq(applications[1].last_name, last_name2, "Expected second last name to be stored correctly; expected '%s', got '%s'", last_name2, applications[1].last_name);
    cr_assert_eq(applications[1].age, age2, "Expected second age to be stored correctly; expected %d, got %d", age2, applications[1].age);
    cr_assert_eq(applications[1].project_count, project_count2, "Expected second project count to be stored correctly; expected %d, got %d", project_count2, applications[1].project_count);
    cr_assert_eq(applications[1].has_referral, has_referral2, "Expected second referral value to be stored correctly; expected %d, got %d", has_referral2, applications[1].has_referral);

    //check if application count is updated
    cr_assert_eq(app_count, 2, "Expected application count to be updated to 2");

}