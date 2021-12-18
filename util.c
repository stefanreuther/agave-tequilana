/**
  *  \file util.c
  *  \brief Agave Tequilana - Assorted Utilities
  */

#include <string.h>
#include "util.h"

Boolean PlanetHasFCode(Uns16 planetId, const char* expected)
{
    char fc[3];
    PlanetFCode(planetId, fc);
    return memcmp(fc, expected, 3) == 0;
}
