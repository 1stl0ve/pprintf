/** @file test.c
 *  @brief A test suite for pprintf().
 *
 *  Iterates through a set of test cases to ensure that
 *  pprintf has constructed the correct strings.
 */
#include "pprintf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define ITALIC "\x1b[3m"
#define BOLD "\x1b[1m"
#define UNDERLINE "\x1b[4m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"
#define NORMAL "\x1b[0m"

/** @brief Construct the expected output string.
 *
 *  @param format A pointer to the ANSI string to apply.
 *  @returns The string "hello %s\n" with ANSI formatting applied.
 */
char *get_expected(char *format) {
    static char *str;
    str = malloc(23*sizeof(char));
    strcpy(str, "hello \x1b[0m");
    strcat(str, format);
    strcat(str, "%s");
    strcat(str, NORMAL);
    strcat(str, "\n");
    return str;
}

/** @brief Runs the test cases.
 *  
 *  This function runs the test cases. The function constructs
 *  the expected output and compares it to the string created
 *  by the pprintf library. If the two strings are the same, it
 *  reports the test case as successful.
 *
 *  @returns Void.
 */
void run_tests() {
    /* Generate the test ANSI formats and the format characters associated with them. */
    char *choices[10] = { RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, BOLD, UNDERLINE, ITALIC };
    char *format_strings[10] = { "R", "G", "Y", "B", "M", "C", "W", "D", "U", "I" };

    char s[] = "world";
    int total_tests = 0;
    int passed_tests = 0;

    /* For each test case, construct the expected string 
     * and compare it to the results. 
     */
    for (int i = 0; i < 10; i++) {
        char *input = malloc(23*sizeof(char));
        sprintf(input, "hello %s%s%s\n", "%", format_strings[i], "s");
        pprintf(input, s);
        char *expected = get_expected(choices[i]);
        char *received = get_pprintf_value(input, s);
        if ((strcmp(expected, received)) == 0) {
            passed_tests++;
        } 
        total_tests++;

        /* Memory management after each test. */
        free(received);
        free(input);
        free(expected);
    }

    /* Print out the results. */
    printf("TOTAL TESTS: %d\n", total_tests);
    printf("TESTS: PASSED: %d\n", passed_tests);
}

int main() {
    run_tests();
    return 0;
}
