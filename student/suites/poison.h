/**
 * @file poison.h
 * @brief Poisons standard library functions that would trivialize the assignment
 * 
 * DO NOT MODIFY THIS FILE
 * 
 * Allowed functions: printf, strlen, strcpy, strncpy, strcmp, strncmp
 * This file poisons functions that might trivialize the project
 */
#ifndef POISON_H
#define POISON_H

// poison string.h functions (except allowed)
#pragma GCC poison memcpy memmove memset memcmp
#pragma GCC poison strcat strncat       
#pragma GCC poison strchr strrchr
#pragma GCC poison strstr        
#pragma GCC poison strdup strndup      
#pragma GCC poison strtok strtok_r 

// poison stdio.h functions (except allowed)
#pragma GCC poison fprintf sprintf snprintf vprintf vfprintf vsprintf vsnprintf
#pragma GCC poison scanf fscanf sscanf vscanf vfscanf vsscanf
#pragma GCC poison fgetc fgets fputc fputs getc getchar gets putc putchar puts ungetc

// poison stdlib.h (dynamic allocation)
#pragma GCC poison malloc calloc realloc free

// poison math.h functinos
#pragma GCC poison fmod fmodf fmodl

// poison sorting functinos
#pragma GCC poison qsort bsearch

// poison ctype.h (character manipulation)
#pragma GCC poison isalnum isalpha islower isupper isdigit isxdigit 
#pragma GCC poison iscntrl isgraph isspace isblank isprint ispunct 
#pragma GCC poison tolower toupper

// poison system/exec functions for security
#pragma GCC poison system execl execle execlp execv execve execvp fexecve

// Remove function prototypes from ag_utils.h.
// This creates a compiler error if unauthorized
// access to ag_utils.h is attempted.
//
// This doesn't patch authorized access completely;
// but it causes a compile error rather than a linker error.
#define AG_UTILS_H

#endif
