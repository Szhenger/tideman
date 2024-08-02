#define _GNU_SOURCE

#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cs50.h"

// Disable warnings from some compilers about the way we use variadic arguments
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"

/**
 * Number of strings allocated by get_string.
 */
static size_t allocations = 0;

/**
 * Array of strings allocated by get_string.
 */
static string *strings = NULL;

/**
 * Prompts user for a line of text from standard input and returns
 * it as a string (char *), sans trailing line ending. Supports
 * CR (\r), LF (\n), and CRLF (\r\n) as line endings. If user
 * inputs only a line ending, returns "", not NULL. Returns NULL
 * upon error or no input whatsoever (i.e., just EOF). Stores string
 * on heap, but library's destructor frees memory on program's exit.
 */
#undef get_string
string get_string(va_list *args, const char *format, ...)
{
    // Check whether we have room for another string
    if (allocations == SIZE_MAX / sizeof (string))
    {
        return NULL;
    }

    // Growable buffer for characters
    string buffer = NULL;

    // Capacity of buffer
    size_t capacity = 0;

    // Number of characters actually in buffer
    size_t size = 0;

    // Character read or EOF
    int c;

    // Prompt user
    if (format != NULL)
    {
        // Initialize variadic argument list
        va_list ap;

        // Students' code will pass in printf-like arguments as variadic
        // parameters. The student-facing get_string macro always sets args to
        // NULL. In this case, we initialize the list of variadic parameters
        // the standard way with va_start.
        if (args == NULL)
        {
            va_start(ap, format);
        }

        // When functions in this library call get_string they will have
        // already stored their variadic parameters in a `va_list` and so they
        // just pass that in by pointer.
        else
        {
            // Put a copy of argument list in ap so it is not consumed by vprintf
            va_copy(ap, *args);
        }

        // Print prompt
        vprintf(format, ap);

        // Clean up argument list
        va_end(ap);
    }

    // Iteratively get characters from standard input, checking for CR (Mac OS), LF (Linux), and CRLF (Windows)
    while ((c = fgetc(stdin)) != '\r' && c != '\n' && c != EOF)
    {
        // Grow buffer if necessary
        if (size + 1 > capacity)
        {
            // Increment buffer's capacity if possible
            if (capacity < SIZE_MAX)
            {
                capacity++;
            }
            else
            {
                free(buffer);
                return NULL;
            }

            // Extend buffer's capacity
            string temp = realloc(buffer, capacity);
            if (temp == NULL)
            {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }

        // Append current character to buffer
        buffer[size++] = c;
    }

    // Check whether user provided no input
    if (size == 0 && c == EOF)
    {
        return NULL;
    }

    // Check whether user provided too much input (leaving no room for trailing NUL)
    if (size == SIZE_MAX)
    {
        free(buffer);
        return NULL;
    }

    // If last character read was CR, try to read LF as well
    if (c == '\r' && (c = fgetc(stdin)) != '\n')
    {
        // Return NULL if character can't be pushed back onto standard input
        if (c != EOF && ungetc(c, stdin) == EOF)
        {
            free(buffer);
            return NULL;
        }
    }

    // Minimize buffer
    string s = realloc(buffer, size + 1);
    if (s == NULL)
    {
        free(buffer);
        return NULL;
    }

    // Terminate string
    s[size] = '\0';

    // Resize array so as to append string
    string *tmp = realloc(strings, sizeof (string) * (allocations + 1));
    if (tmp == NULL)
    {
        free(s);
        return NULL;
    }
    strings = tmp;

    // Append string to array
    strings[allocations] = s;
    allocations++;

    // Return string
    return s;
}

/**
 * Called automatically after execution exits main.
 */
static void teardown(void)
{
    // Free library's strings
    if (strings != NULL)
    {
        for (size_t i = 0; i < allocations; i++)
        {
            free(strings[i]);
        }
        free(strings);
    }
}

/**
 * Preprocessor magic to make initializers work somewhat portably
 * Modified from http://stackoverflow.com/questions/1113409/attribute-constructor-equivalent-in-vc
 */
#if defined (_MSC_VER) // MSVC
    #pragma section(".CRT$XCU",read)
    #define INITIALIZER_(FUNC,PREFIX) \
        static void FUNC(void); \
        __declspec(allocate(".CRT$XCU")) void (*FUNC##_)(void) = FUNC; \
        __pragma(comment(linker,"/include:" PREFIX #FUNC "_")) \
        static void FUNC(void)
    #ifdef _WIN64
        #define INITIALIZER(FUNC) INITIALIZER_(FUNC,"")
    #else
        #define INITIALIZER(FUNC) INITIALIZER_(FUNC,"_")
    #endif
#elif defined (__GNUC__) // GCC, Clang, MinGW
    #define INITIALIZER(FUNC) \
        static void FUNC(void) __attribute__((constructor)); \
        static void FUNC(void)
#else
    #error The CS50 library requires some compiler-specific features, \
           but we do not recognize this compiler/version. Please file an issue at \
           https://github.com/cs50/libcs50
#endif

/**
 * Called automatically before execution enters main.
 */
INITIALIZER(setup)
{
    // Disable buffering for standard output
    setvbuf(stdout, NULL, _IONBF, 0);
    atexit(teardown);
}

// Re-enable warnings
#pragma GCC diagnostic pop
