/* console.h
 * Copyright (C) 2021 Jad Altahan (https://github.com/xv)
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct {
#ifdef _WIN32
    HANDLE std_handle;
    CONSOLE_SCREEN_BUFFER_INFO console_info;
#endif
} ConsoleWindow;

// https://ss64.com/nt/color.html
typedef enum {
    COLOR_NONE = -1,
    COLOR_BLACK = 0x0,
#ifdef _WIN32
    COLOR_BLUE = 0x1,
    COLOR_GREEN = 0x2,
    COLOR_AQUA = 0x3,
    COLOR_RED = 0x4,
    COLOR_PURPLE = 0x5,
    COLOR_YELLOW = 0x6,
    COLOR_WHITE = 0x7,
    COLOR_GRAY = 0x8,
    COLOR_LIGHT_BLUE = 0x9,
    COLOR_LIGHT_GREEN = 0xa,
    COLOR_LIGHT_AQUA = 0xb,
    COLOR_LIGHT_RED = 0xc,
    COLOR_LIGHT_PURPLE = 0xd,
    COLOR_LGIHT_YELLOW = 0xe,
    COLOR_BRIGHT_WHITE = 0xf
#else
    COLOR_RED = 0x1,
    COLOR_GREEN = 0x2,
    COLOR_YELLOW = 0x3,
    COLOR_BLUE = 0x4,
    COLOR_MAGENTA = 0x5,
    COLOR_CYAN = 0x6,
    COLOR_WHITE = 0x7
#endif
} ConsoleColor;

ConsoleWindow* console_init();
void console_set_colors(ConsoleWindow* ti, ConsoleColor fg, ConsoleColor bg);
void console_reset_colors(ConsoleWindow* ti);