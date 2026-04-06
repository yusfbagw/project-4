/**
 * @file string.h
 * @brief Restricted string.h header for autograding
 * 
 * DO NOT MODIFY THIS FILE
 * 
 * Minimal string.h file that only exposes standard lib functions that 
 * students are allowed to use in Project 4. The Makefile uses the
 * -I flag to ensure this custom header is included instead of the system string.h
 * during violation checking.
 * 
 */
#ifndef _CUSTOM_STRING_H
#define _CUSTOM_STRING_H

#include <stddef.h>

#define UNUSED_PARAM(x) ((void)x) // This macro is only used for turning off compiler errors initially

// functions that students are allowed to use
size_t strlen(const char *s);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

#endif
