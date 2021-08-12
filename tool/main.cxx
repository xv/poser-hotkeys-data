/* main.cxx
 * Copyright (C) 2021 Jad Altahan (https://github.com/xv)
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include <iomanip>
#include "console.h"
#include "strutil.h"

#define PROG_HAS_ARG(arg) argv[i] && strcmp(argv[i], arg) == 0

Console console;

constexpr int ALPHABET_LETTERS = 26;
static char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

struct Config
{
    Config()
    {
        packPairs = 0;
        zeroPad = 1;
    }

    std::string packName, packSize, animName;
    int packPairs, zeroPad;
};

/**
 * @brief Outputs program usage details.
 */
static void PrintUsageInfo()
{
    std::cout << "Usage:\n  pg [-an <AnimationName>] [options]\n\
  The order of specifying arguments does not matter.\n\n\
  Options:\n\
     -pn [pack name.] Sets the pack name. This is what you see in Poser Hotkeys'\n\
     <string>         MCM when you try to select a pose pack from a specific\n\
                      posing mod. If this argument is not specified, then\n\
                      <AnimationName> will be used.\n\n\
     -ps [pack size.] Sets the number of items (aka animations) in the pack. The\n\
     <int>            incrementation begins from 1 by default. You can specify a\n\
                      number range by using A:Z, where A and Z represent starting\n\
                      and ending values.\n\n\
     -pp [pack pairs] Creates pairs for each item (aka animation) in the pack by\n\
     <int>            appending a lowercase alphabet letter to the end of each\n\
                      item. For example, specifying a pair of two for a pack\n\
                      size of two will create [anim1a, anim1b, anim2a, anim2b].\n\n\
     -PP [pack pairs] Performs the same function as [-pp] except that it appends\n\
     <int>            an UPPERCASE alphabet letter instead of lowercase.\n\n\
     -zp [zero pad..] Pads the incremental integer part of <AnimationName> with\n\
     <int>            leading zeroes.\n";
}

/**
 * @brief Outputs a single Poser Hotkeys animation configuration line based on
 *        the provided program arguments.
 *
 * @param config The command line configuration of the program from which data
 *               is processed and generated.
 */
void GeneratePack(Config config)
{
    if (config.packName.empty())
    {
        config.packName = config.animName;
        console.Write(Console::Color::COLOR_YELLOW, 
            "[-pn] was auto defaulted to the value of [-an]\n");
    }

    int sizeRange[2] = { 0 };

    if (config.packSize.empty())
    {
        sizeRange[0] = 1;
        sizeRange[1] = 1;
        console.Write(Console::Color::COLOR_YELLOW, 
            "[-ps] was auto defaulted to 1\n");
    }
    else
    {
        int rangeDelim = config.packSize.find(':');
        if (rangeDelim != std::string::npos)
        {
            sizeRange[0] = StringUtil::StrToInt(config.packSize.substr(0, rangeDelim), -1);
            sizeRange[1] = StringUtil::StrToInt(config.packSize.substr(rangeDelim + 1), -1);

            if (sizeRange[0] < 0 || sizeRange[1] < 0)
            {
                console.Write(Console::Color::COLOR_RED,
                    "Invalid [-ps] range: value contains unparseable data\n");
                exit(EXIT_FAILURE);
            }
            else if (sizeRange[0] == 0 && sizeRange[1] == 0)
            {
                console.Write(Console::Color::COLOR_RED,
                    "Invalid [-ps] range: start and end values cannot be both zeroes\n");
                exit(EXIT_FAILURE);
            }
            else if (sizeRange[0] >= 0 && sizeRange[1] < sizeRange[0])
            {
                console.Write(Console::Color::COLOR_RED,
                    "Invalid pack [-ps] range: end value is less than start value");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            int ps = StringUtil::StrToInt(config.packSize, -1);
            if (ps < 1)
            {
                console.Write(Console::Color::COLOR_RED,
                    "Invalid pack size: value is less than 1 or contains unparseable data\n");
                exit(EXIT_FAILURE);
            }

            sizeRange[0] = 1;
            sizeRange[1] = ps;
        }
    }

    if (config.packPairs > ALPHABET_LETTERS)
    {
        console.Write(Console::Color::COLOR_YELLOW,
            "[-pp] is greater than 26 and was ignored\n");
        config.packPairs = 0;
    }

    StringUtil::ToLower(config.packName);
    std::cout << "\"" << config.packName << "\" : [ ";

    for (int i = sizeRange[0]; i <= sizeRange[1]; i++)
    {
        if (config.packPairs < 2)
        {
            std::cout << "\"" << config.animName 
                      << std::setw(config.zeroPad) << std::setfill('0') << i 
                      << "\", ";

            continue;
        }
    
        for (int j = 0; j < config.packPairs; j++)
        {
            std::cout << "\"" << config.animName 
                      << std::setw(config.zeroPad) << std::setfill('0') << i
                      << alphabet[j]
                      << "\", ";
        }
    }

    std::cout << "\b\b ]\n";
}

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);

    if (argc < 2)
    {
        console.Write(Console::Color::COLOR_RED, 
            "No argument is supplied\nExecute <pg ?> to print usage info\n");
        exit(EXIT_FAILURE);
    }

    Config config;

    for (int i = 1; i < argc; i++)
    {
        char* value = argv[i + 1];
        if      (PROG_HAS_ARG("-an") && value != nullptr)
            config.animName = value;
        else if (PROG_HAS_ARG("-pn") && value != nullptr)
            config.packName = value;
        else if (PROG_HAS_ARG("-ps") && value != nullptr)
            config.packSize = value;
        else if (PROG_HAS_ARG("-pp") && value != nullptr)
            config.packPairs = StringUtil::StrToInt(value, 0);
        else if (PROG_HAS_ARG("-PP") && value != nullptr)
        {
            config.packPairs = StringUtil::StrToInt(value, 0);
            for (int i = 0; i < ALPHABET_LETTERS; i++)
                alphabet[i] ^= 32;
        }
        else if (PROG_HAS_ARG("-zp") && value != nullptr)
            config.zeroPad = StringUtil::StrToInt(value, 1);
        else if (PROG_HAS_ARG("?")) {
            // Ignore the argument if it is not the only one
            if (argc > 2)
                continue;

            PrintUsageInfo();
            exit(EXIT_SUCCESS);
        }
    }

    if (config.animName.empty())
    {
        console.Write(Console::Color::COLOR_RED, "[-an] is not set\n");
        exit(EXIT_FAILURE);
    }

    GeneratePack(config);

    return EXIT_SUCCESS;
}