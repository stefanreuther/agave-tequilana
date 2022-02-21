/**
  *  \file config.c
  *  \brief Agave Tequilana - Configuration Handling
  */

#include <stddef.h>
#include <string.h>
#include <strings.h>        // strcasecmp according to SuS
#include "config.h"

/*
 *  Definition of config layout
 */

const char*const CONFIG_FILE_NAME = "cactus.ini";
const char*const CONFIG_FILE_SECTION = "CACTUS";

enum Type {
    tBoolean,
    tInt16
};

struct Definition {
    const char* Name;
    enum Type   Type : 16;
    size_t      Offset : 16;
};

#define CONFIG(type, x) { #x, t##type, offsetof(struct Config, x) }

static const struct Definition CONFIG_DEFINITION[] = {
    CONFIG(Boolean, KeepCactus),
    CONFIG(Boolean, ProcessMessages),
    CONFIG(Int16, TurnScore),
    CONFIG(Int16, TurnOwnerScore),
    CONFIG(Int16, TurnPlusScore),
    CONFIG(Int16, TurnMinusScore),
    CONFIG(Int16, CaptureScore),
    CONFIG(Int16, LostScore),
    CONFIG(Int16, DeadScore),
    CONFIG(Boolean, NeedBase),
    CONFIG(Boolean, RebuildCactus),
    CONFIG(Int16, ClansRequired),
    CONFIG(Int16, CactusLimit),
    CONFIG(Int16, CostAdditive),
    CONFIG(Int16, CostMultiplier),
    CONFIG(Int16, CostPower),
    CONFIG(Int16, MinScore),
    CONFIG(Boolean, EnableFinish),
    CONFIG(Int16, VoteTurn),
    CONFIG(Int16, FinishPercent),
    CONFIG(Int16, FinishScore),
};

/*
 *  Internal
 *
 *  For simplicity, we accept `BoolOption=1` or `IntOption=No`.
 *  The latter is documented for `CactusLimit`.
 */

static struct Config* gConfig;

static Boolean AssignInt16(Int16* p, const char* value)
{
    long parsed;
    char* rem;

    parsed = strtol(value, &rem, 10);
    if ((Int16) parsed == parsed && rem != value && rem[strspn(rem, " \t")] == '\0') {
        *p = (Int16) parsed;
        return True;
    } else if (strcasecmp(value, "yes") == 0 || strcasecmp(value, "true") == 0) {
        *p = 1;
        return True;
    } else if (strcasecmp(value, "no") == 0 || strcasecmp(value, "false") == 0) {
        *p = 0;
        return True;
    } else {
        return False;
    }
}

static Boolean AssignBoolean(Boolean* p, const char* value)
{
    Int16 v;
    if (AssignInt16(&v, value) && (v == 0 || v == 1)) {
        *p = (v != 0);
        return True;
    } else {
        return False;
    }
}

static Boolean AssignGlobalConfig(const char* lhs, char* rhs, const char* line)
{
    size_t i;
    (void) line;
    if (!lhs || !rhs) {
        return True;
    }
    for (i = 0; i < sizeof(CONFIG_DEFINITION)/sizeof(CONFIG_DEFINITION[0]); ++i) {
        const struct Definition* def = &CONFIG_DEFINITION[i];
        if (strcasecmp(lhs, def->Name) == 0) {
            switch (def->Type) {
             case tBoolean:
                return AssignBoolean((Boolean*) ((char*)gConfig + def->Offset), rhs);
             case tInt16:
                return AssignInt16((Int16*) ((char*)gConfig + def->Offset), rhs);
            }
        }
    }
    return False;
}


/*
 *  Public Interface
 */

void Config_Init(struct Config* p)
{
    // General
    p->KeepCactus = False;
    p->ProcessMessages = True;

    // Scoring
    p->TurnScore = 1;
    p->TurnOwnerScore = 1;
    p->TurnPlusScore = 1;
    p->TurnMinusScore = -1;
    p->CaptureScore = 10;
    p->LostScore = -15;
    p->DeadScore = -25;

    // Building
    p->NeedBase = False;
    p->RebuildCactus = False;
    p->ClansRequired = 0;
    p->CactusLimit = 0;
    p->CostAdditive = 0;
    p->CostMultiplier = 0;
    p->CostPower = 0;
    p->MinScore = -32768;

    // Voting
    p->EnableFinish = True;
    p->VoteTurn = 65;
    p->FinishPercent = 66;
    p->FinishScore = 2000;
}

void Config_Load(struct Config* p)
{
    FILE* f;
    struct Config* prev;

    Config_Init(p);

    f = OpenInputFile(CONFIG_FILE_NAME, GAME_DIR_ONLY | TEXT_MODE | NO_MISSING_ERROR);
    if (f == NULL) {
        Warning("Configuration file (%s) not found, using defaults.", CONFIG_FILE_NAME);
        return;
    }

    // PDK is not reentrant, but we pretend to be.
    prev = gConfig;
    gConfig = p;
    ConfigFileReader(f, CONFIG_FILE_NAME, CONFIG_FILE_SECTION, True, AssignGlobalConfig);
    gConfig = prev;

    fclose(f);
}

void Config_Format(const struct Config* p, void func(void* state, const char* name, const char* value), void* state)
{
    size_t i;
    for (i = 0; i < sizeof(CONFIG_DEFINITION)/sizeof(CONFIG_DEFINITION[0]); ++i) {
        const struct Definition* def = &CONFIG_DEFINITION[i];
        switch (def->Type) {
         case tBoolean: {
            Boolean value = *(Boolean*) ((char*)p + def->Offset);
            func(state, def->Name, value ? "Yes" : "No");
            break;
         }
         case tInt16: {
            Int16 value = *(Int16*) ((char*)p + def->Offset);
            char tmp[20];
            snprintf(tmp, sizeof(tmp), "%d", value);
            func(state, def->Name, tmp);
            break;
         }
        }
    }
}
