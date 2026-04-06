// DO NOT MODIFY THIS FILE
#ifndef ASSEMBLY
#define ASSEMBLY

#define UNUSED(x) ((void)x) // This macro is only used for turning off compiler errors initially

#define SUCCESS 0
#define ERROR -1
#define FAILURE -2
#define INCOMPLETE -3

// Function prototypes for assembly.c

int longestWordleStreak(int array[], int length);
int validSudokuRow(char row[]);
void createLetterBoxed(char* strings[], int length);
int wordleScore(char guess[], char solution[]);
int validSpellingBee(char word[], char alphabet[], char centerChar);
int octalStringToInt(char octalStr[], int length);

#endif
