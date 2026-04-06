#include "string.h"
#include "stdio.h"
#include "humanresources.h"

struct Application applications[MAX_APPLICATIONS_LEN];
int app_count;

/*
* This function adds an application to the array
* Returns SUCCESS if the application was added successfully
*
* You must copy the provided strings into the new struct and set the integer fields
* The new applicant's experiences array should be initialized as empty
* Returns ERROR if the array is full or the application cannot be added
*/
int addApplication(const char *first_name, const char *last_name, int age, int project_count, int has_referral){
    UNUSED(first_name);
    UNUSED(last_name);
    UNUSED(age);
    UNUSED(project_count);
    UNUSED(has_referral);
    return INCOMPLETE;
}

/*
* This function calculates a rating for each application based on its features
* This function should return the calculated rating for the application, or ERROR if the application is invalid
*
* Formula for rating is as follows: Experience Value + Project Value + Referral Value
*
* Experience value is calculated with (experience_type weight * years worked), sum all experience values
* Each experience_type has an integer weight associated with experience type
* Each project adds 5 points to the rating, but 0 projects subtracts 5 points from the rating
* If the applicant has a referral, add 10 points to the rating
* Make sure that there is no negative rating, if the rating is negative, return 0
*/
int calculateRating(struct Application *app) {
    UNUSED(app);
    return INCOMPLETE;
}

/*
* This function determines if an applicant is overqualified for a given role
* Returns SUCCESS if applicant is overqualified, FAILURE if they are not, and ERROR if the application is invalid
*
* Applicant is overqualified if their most recent experience position is higher than the given role
* For example, if role is MANAGER, INTERN and MANAGER are not overqualified, but CEO is overqualified
* 
* Note that the experiences array is not guaranteed to be in order
* so you must iterate through to find the most recent experience
*/
int isOverqualified(struct Application *app, enum ExperienceType role) {
    UNUSED(app);
    UNUSED(role);
    return INCOMPLETE;
}

/*
* This function makes a decision on whether to accept an applicant based on their rating and experience
* Returns SUCCESS if the applicant is accepted, FAILURE if they are rejected, and ERROR if the application is invalid
*
* If applicant's rating is below the given minimum rating, they should be rejected
* If applicant is overqualified for the given role, they should be rejected
* Otherwise, they should be accepted
*/
int makeDecision(struct Application *app, int min_rating, enum ExperienceType role) {
    UNUSED(app);
    UNUSED(min_rating);
    UNUSED(role);
    return INCOMPLETE;
}

/*
* This function adds an experience to an applicant's application
* Returns SUCCESS if the experience was added successfully, and ERROR if the experience is invalid
* 
* Must find application with given first and last name, if no such application exists, return FAILURE
*/
int addExperience(const char *first_name, const char *last_name, const char *company_name, int start_year, int end_year, enum ExperienceType experience_type) {
    UNUSED(first_name);
    UNUSED(last_name);
    UNUSED(company_name);
    UNUSED(start_year);
    UNUSED(end_year);
    UNUSED(experience_type);
    return INCOMPLETE;
}

/*
* This function checks for duplicate applications in the applications array
* Two applications are considered duplicates if they have the same first name, last name, and age
*
* Return SUCCESS if there are duplicates, and FAILURE if there are no duplicates
*/ 
int findDuplicates(void){
    return INCOMPLETE;
}

/*
* Given an applicant's name, set their referral status to true (1)
* Return SUCCESS if referral was given successfully, FAILURE if no such applicant exists, and ERROR if input is invalid
*/
int giveReferral(const char *first_name, const char *last_name) {
    UNUSED(first_name);
    UNUSED(last_name);
    return INCOMPLETE;
}

/*
* This function simulates mass layoffs by removing some applications
*
* Remove applicants whose most recent experience is an intern
* Remove applicants who have less than 3 projects
* Remove applicants whose first name starts with a "J" or "T"
*
* Make sure there are no gaps in the applications array after removing applicants
* Update the application count accordingly
*/
int massLayoffs(void) { 
    return INCOMPLETE;
}

/*
* This function searches for applicants who have worked at a given company and adds their names to the results array
* The results array is a 2D array of characters, where each row can hold a full name (first and last name combined)
* Should return number of applicants who worked at the company, or ERROR if input is invalid

* Note that an applicant may have multiple experiences at the same company, but should only be counted once
* Use the format "FirstName LastName" for each applicant in the results array
*/
int searchByCompany(const char *company, char results[][MAX_APPLICANT_NAME_LEN * 2 + 2]) {
    UNUSED(company);
    UNUSED(results);
    return INCOMPLETE;
}

/*
* This function searches for applicants who have a resume gap and adds their names to the results array
* A resume gap is defined as a period of 3 years or more between consecutive experiences
* The results array is a 2D array of characters, where each row can hold a full name (first and last name combined)
* Should return number of applicants who have a resume gap, or ERROR if input is invalid

* Note that an applicant may have multiple resume gaps, but should only be counted once
* Use the format "FirstName LastName" for each applicant in the results array
*/
int searchByResumeGap(char results[][MAX_APPLICANT_NAME_LEN * 2 + 2]) {
    UNUSED(results);
    return INCOMPLETE;
}

/*
* This function sorts the applications array
* Should sort in descending order by rating, so the highest rated application is at the front of the array
* If two applications have the same rating, sort them in descending order by project count
*
* You may use any sorting algorithm you like, as long as the array is sorted correctly at the end
* Return SUCCESS upon completion
*/
int sortByRating(void) {
    return INCOMPLETE;
}

/*
 * This function prints out the details of a single application.
 * For simplicity you can print someone's name, age, and project count
 * You are free to format this however you like. Get creative!
 * This is to make sure you understand how to use printf statements
 * (Note: This function will not be strictly autograded).
 */
void printApplication(struct Application *app) {
    UNUSED(app);
    return;
}