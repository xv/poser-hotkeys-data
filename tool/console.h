/* console.h
 * Copyright (C) 2021 Jad Altahan (https://github.com/xv)
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <iostream>

class Console
{
public:
    struct Screen
    {
        HANDLE handle;
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    };

    // https://ss64.com/nt/color.html
    enum class Color : int
    {
        COLOR_NONE = -1,
        COLOR_BLACK = 0x0,
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
    };

    Console();
    ~Console();
    
    /**
     * @brief Changes the foreground and background colors of the console output.
     *
     * @param fg The desired foreground color.
     * @param bg The desired background color.
     */
    void SetTextColor(Color fg, Color bg);

    /**
     * @brief Resets the foreground and background colors of the console output to
     *        their default system values.
     */
    void ResetTextColor();

    /**
     * @brief Prints a string to standard output.
     * 
     * @tparam ...Args
     * @param c The color of the output.
     * @param ...args Arguments to be included in the buffer stream.
     */
    template<typename ...Args>
    void Write(Color c, Args &&...args)
    {
        SetTextColor(c, Console::Color::COLOR_NONE);
        (std::cout << ... << args);
        ResetTextColor();
    }

private:
    Screen* m_pWi;
};