/**
  *  \file util.c
  *  \brief Agave Tequilana - Assorted Utilities
  */

#include <string.h>
#include <ctype.h>
#include "util.h"

Boolean PlanetHasFCode(Uns16 planetId, const char* expected)
{
    char fc[3];
    PlanetFCode(planetId, fc);
    return memcmp(fc, expected, 3) == 0;
}

const char* StrStartsWith(const char* line, const char* expectedPrefix)
{
    // Check prefix
    while (*expectedPrefix != '\0') {
        if (tolower((unsigned char) *expectedPrefix) != tolower((unsigned char) *line)) {
            return 0;
        }
        ++expectedPrefix;
        ++line;
    }

    // Check additional whitespace
    while (*line == ' ') {
        ++line;
    }
    return line;
}
