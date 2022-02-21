/**
  *  \file util.h
  *  \brief Agave Tequilana - Assorted Utilities
  */
#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <phostpdk.h>

/** Check for planet friendly code.
    Code is checked case-sensitively.

    @param [in] planetId Planet Id
    @param [in] expected Expected friendly code (3-character string)

    @return true on match */
Boolean PlanetHasFCode(Uns16 planetId, const char* expected);

/** Check for prefix, case-insensitive.
    If @c line starts with @c expectedPrefix, returns a pointer to the suffix (with whitespace removed).

    @param [in] line            Line to check
    @param [in] expectedPrefix  Prefix to look for

    @return Suffix on success, otherwise null */
const char* StrStartsWith(const char* line, const char* expectedPrefix);


#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

#endif
