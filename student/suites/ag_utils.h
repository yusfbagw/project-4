#ifndef AG_UTILS_H
#define AG_UTILS_H

#include <stdio.h>

#include "../humanresources.h"
// The Makefile script + Criterion bugs are causing weird interactions.
// `make run-tests` cannot be ^C'd (it has to be ^Z'd).
//
// As such, we need to impose a timeout so that tests will eventually stop
// in cases of infinite loops.
//
// However, we do not want to impose a timeout when debugging with GDB
// (since that wouldn't be a fun debugging experience).
//
// One way of doing this is defining ./tests --timeout N in make run-tests;
// however, this requires specifying a non-default timeout (because Criterion bug).
// You can't set +INFINITY because +INFINITY (also due to a bug) acts as a 0s timeout.
#define UNREASONABLY_LONG 86400




// These are helper functions for the autograder.
// You may not use these for your solution.

// assembly.c helper functions

int helpervalidSpellingBee(char word[], char alphabet[], char centerChar);
int helperOctalStringToInt(char octleString[], int length);
// humanresources.c helper functions

// Directly inserts a blank application into the global array (bypasses addApplication).
// Use this to set up test state without depending on the student's addApplication.
void helper_insert_application(const char *first_name, const char *last_name);

int helper_addApplication(const char *first_name, const char *last_name, int age, int project_count, int has_referral);
int helper_addExperience(const char *first_name, const char *last_name, const char *company_name, int start_year, int end_year, enum ExperienceType experience_type);
int helper_findDuplicates(void);
void helper_reset_applications(void);
void helper_add_app(const char *first, const char *last, int age, int project_count, int has_referral);
void helper_add_exp(int app_idx, const char *company, int start, int end, enum ExperienceType type);
int helper_calculate_rating(struct Application *app);
void helper_format_name(char *buf, const char *first, const char *last);

#endif
