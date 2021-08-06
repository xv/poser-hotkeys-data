/* main.c
 * Copyright (C) 2021 Jad Altahan (https://github.com/xv)
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "console.h"

#ifdef _WIN32
    #include <io.h>
#else
    #include <unistd.h>
#endif

static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

#define digit(ch) (ch > 0x2f && ch < 0x3a)

#define has_arg(arg) argv[i] && strcmp(argv[i], arg) == 0

#define puts(str) fputs((str), stdout)

#define printf_error(fmt, ...) {\
    console_set_colors(cw, COLOR_RED, COLOR_NONE);\
    fprintf(stderr, fmt, ##__VA_ARGS__);\
    console_reset_colors(cw);\
};

#define printf_warning(fmt, ...) {\
    console_set_colors(cw, COLOR_YELLOW, COLOR_NONE);\
    printf(fmt, ##__VA_ARGS__);\
    console_reset_colors(cw);\
};

#define parse_int_arg(arg, in) {\
    if (argv[arg] && digit(argv[arg][0])) {\
        in = parse_int(argv[arg], in);\
    } else {\
        printf_warning("Ignored argument [%s] (invalid value '%s')\n",\
                        argv[arg - 1], argv[arg]);\
        continue;\
    }\
};

ConsoleWindow* cw;

struct Config {
    char* packName, * packSize;
    int packPairs;
};

/**
 * @brief Parses a given string as an integer.
 * 
 * @param str String to parse.
 * @param def Default value to return if parsing fails.
 * 
 * @return If parsing succeeds, the function returns the converted number as a
 *         long integer value; otherwise, a default value provided in param
 *         `def` is returned.
 */
long parse_int(const char* str, long def) {
    if (str == NULL)
        return def;

    char* end;
    long p = strtol(str, &end, 0);

    if (end == str)
        return def;

    return p;
}

/**
 * @brief Duplicates a given string in memory.
 * 
 * @param src The source string to duplciate.
 * @return Pointer to the duplicated string, or NULL if memory allocation fails.
 */
char* string_duplicate(const char* src) {
    size_t len = strlen(src) + 1;
    char* dst = malloc(len);

    if (dst == NULL)
        return NULL;

    return (char*)memcpy(dst, src, len);
}

/**
 * Converts a given string to lowercase.
 * @param str The string to manipulate.
 */
void string_lowercase(char* str)
{
    while (*str)
    {
        if (*str >= 'A' && *str <= 'Z')
            *str += 32;
    
        str++;
    }
}

/**
 * @brief Outputs a single Poser Hotkeys animation configuration line based on
 *        the provided program arguments.
 *
 * @param config The command line configuration of the program from which data
 *               is processed and generated.
 */
void generate_pack(struct Config config) {
    char rangeDelim[] = ":";
    int data[2] = { 0 };

    if (strchr(config.packSize, rangeDelim[0]) != NULL) {
        int tc = 0;
        for (char* p = strtok(config.packSize, rangeDelim); p != NULL; p = strtok(NULL, rangeDelim)) {
            if (tc == 2)
                break;
  
            data[tc++] = parse_int(p, -1);
        }

        if (data[0] < 0 || data[1] < 0) {
            printf_error("Invalid pack size range: input contains unparseable data\n");
            return;
        } else if (data[0] == 0 && data[1] == 0) {
            printf_error("Invalid pack size range: start and end values cannot be both zeroes\n");
            return;
        } else if (data[0] >= 0 && data[1] < data[0]) {
            printf_error (
                "Invalid pack size range: end value '%i' is less than the start value of '%i'\n", 
                data[1], data[0]
            );
            return;
        }
    } else {
        int end = parse_int(config.packSize, -1);
        if (end < 1) {
            printf_error("Invalid pack size: value cannot be less than 1\n");
            return;
        }

        data[0] = 1;
        data[1] = end;
    }

    char* dup = string_duplicate(config.packName);
    string_lowercase(dup);
    printf("\"%s\" : [ ", dup);
    free(dup);

    for (int i = data[0]; i <= data[1]; i++) {
        if (config.packPairs < 2) {
            printf("\"%s%i\"", config.packName, i);
            puts(i != data[1] ? ", " : " ]");
            continue;
        }

        for (int j = 0; j < config.packPairs; j++)
            printf("\"%s%i%c\", ", config.packName, i, alphabet[j]);
    }
    puts("\b\b ]\n");
}

/**
 * @brief Entry point of the program.
 * 
 * @param argc Number of arguments passed to the program.
 * @param argv Pointer to the first element of an array of argc + 1.
 */
int main(int argc, char* argv[]) {
    cw = console_init();

    if (argc < 2) {
        printf_error("No argument is supplied\nExecute <phpg ?> to print usage info\n");
        exit(EXIT_FAILURE);
    }

    struct Config config = { 0, 0, 0 };

    for (int i = 1; i < argc; ++i) {
        int val = i + 1;
        if      (has_arg("-pn")) { config.packName = argv[val]; }
        else if (has_arg("-ps")) { config.packSize = argv[val]; }
        else if (has_arg("-pp")) { parse_int_arg(val, config.packPairs); }
    }

    generate_pack(config);
    free(cw);

    return EXIT_SUCCESS;
}