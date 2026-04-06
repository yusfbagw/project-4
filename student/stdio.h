/**
 * @file stdio.h
 * @brief Restricted stdio.h header for autograding
 * 
 * DO NOT MODIFY THIS FILE
 * 
 * Minimal stdio.h file that only exposes standard lib functions that 
 * students are allowed to use in Project 4. The Makefile uses the
 * -I flag to ensure this custom header is included instead of the system stdio.h
 * during violation checking.
 * 
 */
#ifndef _CUSTOM_STDIO_H
#define _CUSTOM_STDIO_H

#include <stdio.h> 

// functions that students are allowed to use
int printf(const char *format, ...);


#endif
