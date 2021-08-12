/* strutil.cxx
 * Copyright (C) 2021 Jad Altahan (https://github.com/xv)
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#pragma once
#include "strutil.h"

int StringUtil::StrToInt(std::string str, int def)
{
    size_t pos;
    int ret;
    try
    {
        ret = std::stoi(str, &pos);
        // Force-fail parsing on non-numeric chars
        if (pos != str.size())
            ret = def;
    }
    catch (...) { ret = def; }

    return ret;
}

void StringUtil::ToLower(std::string& str)
{
    std::transform
    (
        str.begin(),
        str.end(),
        str.begin(),
        ::tolower
    );
}