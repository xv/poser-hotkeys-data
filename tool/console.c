/* console.c
 * Copyright (C) 2021 Jad Altahan (https://github.com/xv)
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include "console.h"

ConsoleWindow* console_init() {
    ConsoleWindow* cw = (ConsoleWindow*)calloc(1, sizeof(ConsoleWindow));

#ifdef _WIN32
    if (cw) {
        cw->std_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (cw->std_handle != NULL) {
            if (!GetConsoleScreenBufferInfo(cw->std_handle, &cw->console_info))
                cw->std_handle = NULL;
        }
    }
#endif

    return cw;
}

void console_set_colors(ConsoleWindow* cw, ConsoleColor fg, ConsoleColor bg) {
    if (bg == COLOR_NONE && fg == COLOR_NONE)
        return;

#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(cw->std_handle, &info))
        return;

    WORD color = info.wAttributes;
    if (fg != COLOR_NONE)
        color = (color & 0xFFF0) | fg;

    if (bg != COLOR_NONE)
        color = (color & 0xFF0F) | (bg << 4);

    if (cw->std_handle != NULL)
        SetConsoleTextAttribute(cw->std_handle, color);
#endif
}

void console_reset_colors(ConsoleWindow* cw) {
#ifdef _WIN32
    if (cw == NULL)
        return;

    if (cw->std_handle != NULL)
        SetConsoleTextAttribute(cw->std_handle, cw->console_info.wAttributes);
#endif
}