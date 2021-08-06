/* console.c
 * Copyright (C) 2021 Jad Altahan (https://github.com/xv)
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include "console.h"

ConsoleWindow* console_init()
{
    ConsoleWindow* ti = (ConsoleWindow*)calloc(1, sizeof(ConsoleWindow));

#ifdef _WIN32
    if (ti)
    {
        ti->std_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (ti->std_handle != NULL)
        {
            if (!GetConsoleScreenBufferInfo(ti->std_handle, &ti->console_info))
                ti->std_handle = NULL;
        }
    }
#endif

    return ti;
}

void console_set_colors(ConsoleWindow* ti, ConsoleColor fg, ConsoleColor bg) {
    if (bg == COLOR_NONE && fg == COLOR_NONE)
        return;

#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(ti->std_handle, &info))
        return;

    WORD color = info.wAttributes;
    if (fg != COLOR_NONE)
        color = (color & 0xFFF0) | fg;

    if (bg != COLOR_NONE)
        color = (color & 0xFF0F) | (bg << 4);

    if (ti->std_handle != NULL)
        SetConsoleTextAttribute(ti->std_handle, color);
#endif
}

void console_reset_colors(ConsoleWindow* ti) {
#ifdef _WIN32
    if (ti == NULL)
        return;

    if (ti->std_handle != NULL)
        SetConsoleTextAttribute(ti->std_handle, ti->console_info.wAttributes);
#endif
}