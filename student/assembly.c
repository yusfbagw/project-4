#include "string.h"
#include "stdio.h"
#include "assembly.h"

// All functions are consistent with the Project 3 functions

// This function returns the longest streak of 1s in a given array of 1s and 0s
int longestWordleStreak(int array[], int length) {
    #define MAX(a, b) ((a) > (b) ? (a) : (b))
    // delete the following three lines when you begin work on this function!
    int n = length;
    int currentStreak = 0; 
    int bestStreak = 0;
    
    if (array == NULL || length <= 0){
        return FAILURE;
    }

    for (int i = 0; i < n; i++) {
        if (array[i] == 1){
            currentStreak++;
        }
        else if (currentStreak != 0 ) {
            bestStreak  = MAX(bestStreak, currentStreak);
            currentStreak = 0; 
        }
    }
    bestStreak = MAX(bestStreak, currentStreak);
    
    return bestStreak;
}

// This function iterates through an array of up to 9 characters and checks if there are duplicates
// Returns SUCCESS if there are characters 1-9 with no duplicates, FAILURE otherwise
int validSudokuRow(char row[]) {
    // delete the following two lines when you begin work on this function!
    int visited[9];

    if (row == NULL) {
        return FAILURE;
    }
    for (int i = 0; i < 9; ++i) {
        visited[i] = 0;
    }

    for (int i = 0; i < 9; ++i) {
        visited[row[i] - 1] = 1;
    }

    int sum = 0;
    for (int i = 0; i < 9; ++i) {
        sum += visited[i];
    }

    if (sum == 9) {
        return SUCCESS;
    }
    else {
        return FAILURE;
    }

}

// This functions modifies an array of strings so the first character of each string
// becomes the last character of the previous string
// Ex. Strings = ["HEY", "HELLO", "GOODBYE"]
// createLetterboxed(strings) -> strings = ["HEY", "YELLO", "OOODBYE"]
void createLetterBoxed(char* strings[], int length) {
    // delete the following three lines when you begin work on this function!
    int i = 0;
    int j = 0;

    while(1 == 1) {
        while(strings[i + 1] != '\0') {
            i++;
        }
        j = i + 2;

        if(strings[j] == '\0') {
            break;
        }

        strings[j] = strings[1];

        i = j;
    }
}

// This function takes a 5-character guess and scores against a solution
// Adds +3 for a correct letter in the wrong position
// Adds +10 for a correct letter in the correct position
int wordleScore(char guess[], char solution[]) {
    
    int score = 0;
    int length = 0;
    while(1) {
        if (strlen(guess) != 5 || strlen(solution) != 5) {
            return FAILURE;
        }

        if (guess[length] == '\0') {
            break;
        }
        if (strlen(guess) != 5) {
            return FAILURE;
        }
        guess[length];
        length++;

    }
    if (length != 5) {
        return FAILURE;
    }
    for (int i = 0; i < 5; i++) {
        if (guess[i] == solution[i]) {
            score += 10;
            guess[i] = '\0';
            solution[i] = '\0';
        }
    }

    for (int i = 0; i < 5; ++i) {
        if (guess[i] == '\0') {
            continue;
        }   
        for (int j = 0; j < 5; ++j) {
            if (guess[i] == solution[j]) {
                score += 3;
                solution[j] = '\0';
                break;
            }
        }
    }
    return score;
}

// Given word, determine whether it is a valid spelling bee word
// Valid word must contain center character and only contain letters in alphabet
// Returns SUCCESS if valid, FAILURE otherwise
int validSpellingBee(char word[], char alphabet[], char centerChar) {
    // delete the following four lines when you begin work on this function!
    int alphabetLength = strlen(alphabet);
    int inputLength = 0;
    int centerCharSeen = 0;

    while(1) {
        if (word[inputLength] == '\0') {
            break;
        }
        inputLength++;
    }

    for (int i = 0; i < inputLength; i++) {
        int validChar = 0;
        if (word[i] == centerChar) {
            centerCharSeen = 1;
        }

        for (int j = 0; j < alphabetLength; ++j) {
            if (word[i] == alphabet[j]) {
                validChar = 1;
                break;
            }
        }
        if (validChar == 0) {
            return FAILURE;
        }
    }

    if (centerCharSeen == 0) {
        return FAILURE;
    }
    else {
        return SUCCESS;
    }
}

// Given an unsigned octal string with given length, return its integer value
int octalStringToInt(char octalStr[], int length) {
    // delete the following three lines when you begin work on this function!
    int value = 0;
    int i = 0;
    if (octalStr == NULL || length <= 0) {
        return FAILURE;
    }
    while (i < length) {
        int leftShifts = 3;
        while (leftShifts > 0) {
            value += value;
            leftShifts--;
        }
        int digit = octalStr[i] - 48;
        value += digit;
        i++;
    }
    return value;
}
