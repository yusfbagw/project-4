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
    if (first_name == NULL || last_name == NULL) {
        return ERROR;
    }
    if (strlen(first_name) == 0 || strlen(last_name) == 0) {
        return ERROR;
    }

    if (age < 0 || project_count < 0) {
        return ERROR;
    }

    if (has_referral > 1 || has_referral < 0){
        return ERROR;
    }
    if (strlen(first_name) >= MAX_APPLICANT_NAME_LEN ||  strlen(last_name) >= MAX_APPLICANT_NAME_LEN) {
        return ERROR;
    }

    if (MAX_APPLICATIONS_LEN <= app_count) {
        return ERROR;
    }

    struct Application *pointer = &applications[app_count];

    strcpy(pointer->first_name, first_name);
    strcpy(pointer->last_name, last_name);

    pointer->age = age;
    pointer->project_count = project_count;
    pointer->has_referral = has_referral;

    for (int i = 0; i < MAX_EXPER_LEN; i++) {
        
        pointer->experiences[i].company_name[0] = '\0';
        pointer->experiences[i].start_year = 0;
        pointer->experiences[i].end_year = 0;
        pointer->experiences[i].experience_type = INTERN;
    }
    
    app_count++;
    return SUCCESS;
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
    
    if (app == NULL) {
        return ERROR;
    }

    int yearsWorked;
    int experienceVal;
    int hasRefferal;
    int endYear;
    int startYear;
    int projectCount;
    int rating = 0;

    struct Experience *pointer;

    hasRefferal = app->has_referral;
    projectCount = app->project_count;
    
    for (int i = 0; i < MAX_EXPER_LEN; i++) {
        pointer = &app->experiences[i];

        if (pointer->company_name[0] == '\0') {
            continue; 
        }

        endYear = pointer->end_year;
        startYear = pointer->start_year;
        yearsWorked = (endYear - startYear);

        experienceVal = (pointer->experience_type * yearsWorked);
        rating += experienceVal;
    }
    

    if (hasRefferal) {
        rating += 10;
    }
    if (projectCount == 0) {
        rating -= 5;
    } 
    else {
        rating += projectCount * 5;
    }
    if (rating < 0) {
        return 0;
    }    
    else {
        return rating;
    }
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
    if (app == NULL) {
        return ERROR;
    }
    
    struct Experience *pointer = NULL;
    int recentYear = -256;
    
    
    for (int i = 0; i < MAX_EXPER_LEN; i++) {
        if (app->experiences[i].start_year == 0) {
            continue; //Making sure that we don't have any garbage data
        }

        if (app->experiences[i].end_year > recentYear) {
            recentYear = app->experiences[i].end_year;
            pointer = &app->experiences[i];
        }
    }
    if (pointer == NULL) {
        return FAILURE;
    }
    
    if (pointer->experience_type > role) {
        return SUCCESS;
    }
    else {
        return FAILURE;
    }
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
    if (app == NULL) {
        return ERROR;
    }

    int rating = calculateRating(app);
    int isOverqual = isOverqualified(app, role);
    
    if (rating == ERROR || isOverqual == ERROR) {
        return ERROR;
    }

    if (rating < min_rating) {
        return FAILURE;
    }
    else if (isOverqual == SUCCESS) {
        return FAILURE;
    }
    else {
        return SUCCESS;
    }
}

/*
* This function adds an experience to an applicant's application
* Returns SUCCESS if the experience was added successfully, and ERROR if the experience is invalid
* 
* Must find application with given first and last name, if no such application exists, return FAILURE
*/
int addExperience(const char *first_name, const char *last_name, const char *company_name, int start_year, int end_year, enum ExperienceType experience_type) {
    struct Experience *pointer;
    struct Application *app; // need to init app

    if (first_name == NULL || last_name == NULL || company_name == NULL) {
        return ERROR;
    }
    if (experience_type < INTERN || experience_type > CEO) {
        return ERROR;
    }
    if (start_year > end_year ) {
        return ERROR;
    }
    if (strlen(first_name) == 0 || strlen(last_name) == 0 || strlen(company_name) == 0) {
        return ERROR;
    }
    if (strlen(company_name) >= MAX_COMPANY_LEN) {
        return ERROR;
    }

    for (int i = 0; i < app_count; i++) {
        if (strcmp(applications[i].first_name, first_name) == 0 && strcmp(applications[i].last_name, last_name) == 0) {
            
            for (int j = 0; j < MAX_EXPER_LEN; j++) {
                if (applications[i].experiences[j].start_year == 0) {
                    strcpy(applications[i].experiences[j].company_name, company_name);
                    applications[i].experiences[j].start_year = start_year;
                    applications[i].experiences[j].end_year = end_year;
                    applications[i].experiences[j].experience_type = experience_type;
                    return SUCCESS;
                }
            } 
            return ERROR; 
        }
        
    }
    return FAILURE;
}

/*
* This function checks for duplicate applications in the applications array
* Two applications are considered duplicates if they have the same first name, last name, and age
*
* Return SUCCESS if there are duplicates, and FAILURE if there are no duplicates
*/ 
int findDuplicates(void){
    int aha = FAILURE;
    for (int i = 0; i < app_count; i++) {
        for (int j = i + 1; j < app_count; j++) {
            if (strcmp(applications[i].first_name, applications[j].first_name) == 0 && strcmp(applications[i].last_name, applications[j].last_name) == 0 && applications[i].age == applications[j].age) {
                aha = SUCCESS;
            }
        }
    }
    if (aha == SUCCESS) {
        return SUCCESS;
    }
    else {
        return FAILURE;
    }
}

/*
* Given an applicant's name, set their referral status to true (1)
* Return SUCCESS if referral was given successfully, FAILURE if no such applicant exists, and ERROR if input is invalid
*/
int giveReferral(const char *first_name, const char *last_name) {
    
    if (first_name == NULL || last_name == NULL) {
        return ERROR;
    }
    
    int wasGiven = 0;
    for (int i = 0; i < app_count; i++) {
        if (strcmp(applications[i].first_name, first_name) == 0 && strcmp(applications[i].last_name, last_name) == 0) {
            applications[i].has_referral = 1;
            wasGiven = 1;
            break;
        }
    }
    if (wasGiven == 1) {
        return SUCCESS;
    }
    else {
        return FAILURE;
    }
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
    for (int i = 0; i < app_count; i++) {
        int recentYear = -256;
        enum ExperienceType mostRecentType = INTERN;
        int aha = 0;
        for (int j = 0; j < MAX_EXPER_LEN; j++) {
        
        if (applications[i].experiences[j].start_year == 0) {
            continue; //Making sure that we don't have any garbage data
        }

        if (applications[i].experiences[j].end_year > recentYear) {
            recentYear = applications[i].experiences[j].end_year;
            mostRecentType = applications[i].experiences[j].experience_type;
            aha = 1;
        }

    }
    if (aha == 1 && mostRecentType == INTERN) {
            applications[i] = applications[app_count - 1];
            app_count--;
            i--;
    }
}
     
    for (int i = 0; i < app_count; i++) {
        if (applications[i].project_count < 3) {
             applications[i] = applications[app_count - 1];
            app_count--;
            i--;
        }
    }
    for (int i = 0; i < app_count; i++) {
        if (applications[i].first_name[0] == 'J' || applications[i].first_name[0] == 'T') {
        applications[i] = applications[app_count - 1];
        app_count--;
        i--;
        } 
    }
    return SUCCESS;
}

/*
* This function searches for applicants who have worked at a given company and adds their names to the results array
* The results array is a 2D array of characters, where each row can hold a full name (first and last name combined)
* Should return number of applicants who worked at the company, or ERROR if input is invalid

* Note that an applicant may have multiple experiences at the same company, but should only be counted once
* Use the format "FirstName LastName" for each applicant in the results array
*/
int searchByCompany(const char *company, char results[][MAX_APPLICANT_NAME_LEN * 2 + 2]) {
    //Ex. 2D Array = char array[2][2] = {{'Bob', 'Baker'}, {'Henry', 'Cavill'}}
    int numPeopleWorked = 0;
    
    if (company == NULL || results == NULL) {
        return ERROR;
    }

    for (int i = 0; i < app_count; i++) {
        for (int j = 0; j < MAX_EXPER_LEN; j++) {
            if (applications[i].experiences[j].start_year == 0) {
                continue;  //Making sure that we don't have any garbage data
            }
            if (strcmp(applications[i].experiences[j].company_name, company) == 0) {
                strcpy(results[numPeopleWorked], applications[i].first_name);
                strcat(results[numPeopleWorked], " ");
                strcat(results[numPeopleWorked], applications[i].last_name);
                numPeopleWorked++;
                break;
            }
        }
    }
    return numPeopleWorked;
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
    
    int numPeopleGap = 0;
    if (results == NULL) {
        return ERROR;
    }

    for (int i = 0; i < app_count; i++) {
            int startYears[MAX_EXPER_LEN];
            int endYears[MAX_EXPER_LEN];
            int count = 0;

            for (int j = 0; j < MAX_EXPER_LEN; j++) {
                if (applications[i].experiences[j].start_year == 0) {
                    continue; //Making sure that we don't have any garbage data
                }
                startYears[count] = applications[i].experiences[j].start_year;
                endYears[count] = applications[i].experiences[j].end_year;
                count++;
            }
            
            for (int a = 0; a < count - 1; a++) {
                for (int b = a + 1; b < count; b++) {
                    if (endYears[a] > endYears[b]) {
                        int tmp = endYears[a]; endYears[a] = endYears[b]; endYears[b] = tmp;
                        tmp = startYears[a]; startYears[a] = startYears[b]; startYears[b] = tmp;
                    }
                }
            }

            int hasGap = 0;
            for (int j = 0; j < count - 1; j++) {
                if (startYears[j + 1] - endYears[j] >= 3) {
                    hasGap = 1;
                    break;
                }
            }

            if (hasGap) {
                strcpy(results[numPeopleGap], applications[i].first_name);
                strcat(results[numPeopleGap], " ");
                strcat(results[numPeopleGap], applications[i].last_name);

                numPeopleGap++;
            }
        }
        return numPeopleGap;
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
    int end = app_count - 1;
    int start = 0;

    while (start < end) {
        int lastSwapped = start;
        
        for (int i = 0; i < end; i++) {
            int ratingOne = calculateRating(&applications[i]);
            int ratingTwo = calculateRating(&applications[i + 1]);
            if (ratingOne < ratingTwo || (ratingOne == ratingTwo && applications[i].project_count < applications[i + 1].project_count)) {
                struct Application temp = applications[i];
                applications[i] = applications[i + 1];
                applications[i + 1] = temp;
                
                lastSwapped = i;
            }
        }
        end = lastSwapped;
    }
    return SUCCESS;
}

/*
 * This function prints out the details of a single application.
 * For simplicity you can print someone's name, age, and project count
 * You are free to format this however you like. Get creative!
 * This is to make sure you understand how to use printf statements
 * (Note: This function will not be strictly autograded).
 */
void printApplication(struct Application *app) {
  
    printf("This is the applicant's name:%s %s\n", app->first_name, app->last_name);
    printf("This is the applicant's age: %d\n", app->age);
    printf("This is the applicant's project count:%d\n", app->project_count);
    
}