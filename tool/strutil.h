/* strutil.h
 * Copyright (C) 2021 Jad Altahan (https://github.com/xv)
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#pragma once
#include <string>
#include <algorithm>

namespace StringUtil
{
    /**
     * @brief Parses a given string as an integer.
     *
     * @param str String to parse.
     * @param def Default value to return if parsing fails.
     *
     * @return If parsing succeeds, the function returns the converted number as
     *         a long integer value; otherwise, a default value provided in
     *         param `def` is returned.
     */
    int StrToInt(std::string str, int def);

    /**
     * @brief Processes a given file path and returns its containing directory.
     * @param str The filename to process
     * @return Path of the directory containing the given file.
     */
    std::string GetDirPathFromFilename(const std::string& path);

    /**
     * @brief Converts a given string to lowercase. This modifies the original
     *        input.
     *
     * @param str The string to convert.
     */
    void ToLower(std::string& str);
}