/** @file pprintf.c
 *  @brief A "pretty" printf() wrapper that adds additional formatting features.
 */
#include "pprintf.h"

/** @brief Checks if a character is a standard format string placeholder.
 *
 *  This function checks the value of a character to see if it is a 
 *  standard printf() placeholder. This value would indicate the end of
 *  the pprintf() placeholder.
 *
 *  @param c The character.
 *  @returns True if the character is a standard format character, otherwise false.
 *
 *  NOTE: Currently the only acceptable format character is 's'.
 */
int is_standard_format(char c) {
    return (c == 's' || c == 'c' || c == 'd' || c == '\0') ? 1 : 0;
}

/** @brief Constructs an ANSI string to represent the formatted text.
 *
 *  This function iterates through all of the characters in a pprintf()
 *  format string placeholder and aggregates ANSI strings for each of
 *  the respective format options.
 *
 *  @param input The input string.
 *  @param output A pointer to copy the ANSI strings to.
 *  @param offset A pointer to an integer that tracks how many characters
 *      are in the format string placeholder (ex: %DBUs => 4).
 *  @returns Void.
 */
void getformats(const char *input, char *output, int *offset) {
    /* Allocate space for the ANSI strings. The largest string would be
     * all of the possible options * 5 characters per option. In the event
     * that this is a regular format string, format is initialized to the
     * ANSI string for "normal" text.
     */
    char *format = calloc(5*TOTAL_OPTIONS+1,sizeof(char));
    strcpy(format, "\x1b[0m");

    /* Iterate through each character in the string. If the character is equal to
     * a standard format string placeholder (ex: 's'), then the function has
     * reached the end of the format placeholder. Otherwise, the character is
     * mapped to a corresponding ANSI string. The ANSI strings are aggregated
     * and written to the output pointer.
     */
    for (int i = 0; i < strlen(input); i++) {
        char c = input[i];
        char *format_str = NULL;

        /* Check to see if the end of the placeholder has been reached. */
        if (is_standard_format(c) && c != '\0') {
            break;
        }

        /* Determine what type of format needs to be included. */
        switch (c) {
            case BOLD_C:
                format_str = BOLD;
                break;
            case ITALIC_C:
                format_str = ITALIC;
                break;
            case UNDERLINE_C:
                format_str = UNDERLINE;
                break;
            case BLUE_C:
                format_str = BLUE;
                break;
            case RED_C:
                format_str = RED;
                break;
            case GREEN_C:
                format_str = GREEN;
                break;
            case YELLOW_C:
                format_str = YELLOW;
                break;
            case MAGENTA_C:
                format_str = MAGENTA;
                break;
            case CYAN_C:
                format_str = CYAN;
                break;
            case WHITE_C:
                format_str = WHITE;
                break;
            default:
                /* If the format placeholder contains an 
                 * invalid character, ignore it.
                 */
                format_str = NULL;
                break;
        }
        /* Add the ANSI code to the temporary string. */
        if (format_str) {
            strcat(format, format_str);
        }
        /* Increment the offset. */
        *offset = *offset + 1;
    }
    /* Write the ANSI strings out to the output pointer. */
    strncpy(output, format, strlen(format));
    free(format);
}

/** @brief Returns the length of the format string.
 *
 *  This function returns the length of the input format string,
 *  accounting for the ANSI strings that will be inserted.
 *
 *  @param str The original format string.
 *  @returns The total length of the format string.
 */
int get_str_length(const char *str) {
    int count = 0;
    for (int i = 0; i < strlen(str); i++) {
        char c = str[i];
        if ('%' == c) {
            while (!is_standard_format(c)) {
                i++;
                c = str[i];
                count = count + 5; // Make space for the formatting characters.
            }
            count++; // Make space for the printf format character.
        }
        count++; // Make space for the current character.
    }
    count = count + 4; // Make space for the "normal" string.
    return count;
}

/** @brief Gets the identifier for the printf placeholder.
 *
 *  This function iterates through a string until it finds
 *  a character associated with a standard format string
 *  placeholder (i.e. 's', 'd', 'c', etc). Once it finds
 *  this character, it returns it.
 *
 *  @param str The address of a string.
 *  @returns The character at the end of a format placeholder.
 */
char get_current_type(char **str) {
    /* Iterate through the string. If we see a standard 
     * character, return it. If we see a null byte,
     * we have reached the end of the string. A return 
     * value of '\0' indicates that the string has ended
     * and there are no more arguments to be processed.
     */
    char current = **str;
    for (;;) {
        if (current == '\0') {
            return current;
        }
        if (current == '%') {
            while (!is_standard_format(current)) {
                (*str)++;
                current = **str;
            }
            (*str)++;
            return current;
        }
        (*str)++;
        current = **str;
    }
}

/** @brief Generates a format string containing ANSI escape codes.
 *  
 *  Generates a format string and replaces the placeholders with ANSI
 *  strings for the pprintf features.
 *
 *  @param str The original format string.
 *  @param ... The arguments to the format string.
 *  @returns The updated format string with ANSI codes.
 */
char *pprintf_value(const char* str, va_list args) {
    char current_type = '\0';
    char *type_str = calloc(strlen(str)+1, sizeof(char));
    strcpy(type_str, str);
    char *type_str_addr = type_str;
    /* Find the total length of the resulting string. 
     * This is composed of three steps:
     * 1. Find the length of str without the format characters; they will be replaced
     * 2. Find the total length of all argument strings.
     * 3. Add the previous two numbers together to get the total length.
     */
    int str_len = get_str_length(str); // Length of str without format chars.

    /* Iterate through each arg and sum up the lengths. */
    va_list original;
    va_copy(original, args);
    int arg_len = 0;

    for (;;) {
        current_type = get_current_type(&type_str);
        /* The string has ended. */
        if ('\0' == current_type) {
            break;
        }
        /* The argument is a string. */
        else if ('s' == current_type) {
            char *str_arg = va_arg(args, char *);
            arg_len = arg_len + strlen(str_arg);
        }
        /* The argument is a character. */
        else if ('c' == current_type) {
            va_arg(args, int);
            arg_len++;
        } 
        /* The argument is an integer. */
        else if ('d' == current_type) {
            int value = va_arg(args, int);
            char str_value[20]; // Assume that users will not supply an int greater than 20 digits
            snprintf(str_value, 20, "%d", value);
            arg_len = arg_len + strlen(str_value);
        }
    }

    free(type_str_addr);
    va_end(args);

    /* Find the total length of the new string. */
    int total_len = str_len + arg_len;
    total_len++; // Account for null byte.

    /* Construct a new string and include the formatting. */
    static char *results;
    results = calloc(total_len+2,sizeof(char));
    char *tmp = calloc(5*TOTAL_OPTIONS,sizeof(char));
    int offset = 0;

    /* Iterate through each character in the string. 
     * If the character is '%', get the formatting.
     * After the format placeholder, return formatting
     * back to normal.
     */
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '%') {
            offset = 0;
            getformats(str+i, tmp, &offset);	
            strcat(results, tmp);
            strcat(results, "%");
            strncat(results, str+i+offset, 1);
            strcat(results, "\x1b[0m");
            i = i + offset;
            bzero(tmp,5*TOTAL_OPTIONS); 
        }
        else {
            strncat(results, str+i, 1);
        }
    }

    free(tmp);
    return results;
}

/** @brief Get a pointer to the formatted string.
 *
 *  Returns a pointer to a string that contains all of
 *  the ANSI format codes specified in the format string.
 *
 *  @param str The original format string.
 *  @returns A pointer to the updated string.
 */
char *get_pprintf_value(const char *str, ...) {
    va_list args;
    va_list passed;
    va_start(args, str);
    va_copy(passed, args);
    char *results = pprintf_value(str, passed);
    va_end(args);
    return results;
}

/** @brief Prints a format string with color and text formatting.
 *  
 *  This function serves as a wrapper for vprintf() that allows for
 *  format strings to contain colors and text formatters such as 
 *  bold, italicized, or underlined text. The format characters are
 *  defined within pprintf.h.
 *
 *  @param str The format string.
 *  @param ... The list of arguments to replace format placeholders.
 *  @returns The numer of bytes written to the screen.
 */
int pprintf(const char *str, ...) {
    va_list args;    
    va_list passed;
    va_start(args, str);
    //va_start(passed, str);
    va_copy(passed, args);

    /* Call pprintf_value to add the ANSI codes to the format string */
    char *results = pprintf_value(str, passed);
    //char *results = pprintf_value(str, args);

    /* Call vprintf to pass a var_arg list to printf() */
    int ret = vprintf(results, args);

    /* Clean up and return. */
    //va_end(passed);
    va_end(args);
    free(results);
    return ret;
}

