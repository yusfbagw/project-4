// DO NOT MODIFY THIS FILE
/**
 * @brief Header file for global macros, structures and fields to be used by the
 * user's program.
 */
#ifndef HUMANRESOURCES_H
#define HUMANRESOURCES_H

#define UNUSED(x) ((void)x) // This macro is only used for turning off compiler errors initially

// Sizes for different arrays
#define MAX_COMPANY_LEN 50
#define MAX_APPLICANT_NAME_LEN 50
#define MAX_EXPER_LEN 10
#define MAX_APPLICATIONS_LEN 100

// Success and failure codes for function return
#define SUCCESS 0
#define ERROR -1
#define FAILURE -2
#define INCOMPLETE -3

// Students should add any structs they make here

enum ExperienceType {
    INTERN,
    ENGINEER_JUNIOR,
    ENGINEER_SENIOR,
    MANAGER,
    MANAGER_SENIOR,
    CEO
};

struct Experience {
    char company_name[MAX_COMPANY_LEN];
    int start_year;
    int end_year;
    enum ExperienceType experience_type;
};

struct Application {
    char first_name[MAX_APPLICANT_NAME_LEN];
    char last_name[MAX_APPLICANT_NAME_LEN];
    int age;
    struct Experience experiences[MAX_EXPER_LEN];
    int project_count;
    int has_referral;
};

int addApplication(const char *first_name, const char *last_name, int age, int project_count, int has_referral);
int calculateRating(struct Application *app);
int isOverqualified(struct Application *app, enum ExperienceType role);
int makeDecision(struct Application *app, int min_rating, enum ExperienceType role);
int addExperience(const char *first_name, const char *last_name, const char *company_name, int start_year, int end_year, enum ExperienceType experience_type);
int findDuplicates(void);
int giveReferral(const char *first_name, const char *last_name);
int searchByCompany(const char *company, char results[][MAX_APPLICANT_NAME_LEN * 2 + 2]);
int searchByResumeGap(char results[][MAX_APPLICANT_NAME_LEN * 2 + 2]);
int sortByRating(void);
int massLayoffs(void);
void printApplication(struct Application *app);


extern struct Application applications[MAX_APPLICATIONS_LEN];
extern int app_count;

# endif