/**
  *  \file commands.c
  *  \brief Agave Tequilana - Command Handling
  */

#include <strings.h>
#include <string.h>
#include "commands.h"
#include "message.h"

struct CommandInfo {
    struct State* pState;
    const struct Config* pConfig;
};

static Boolean CheckCommand(Uns16 pRace, const char* pCommand, const char* pArgs, const char* pWholeLine, void* pData)
{
    const struct CommandInfo*const pInfo = pData;

    if (strcasecmp(pCommand, "defhw") == 0 || strcasecmp(pCommand, "build") == 0) {
        // Plant a cactus
        char* rem;
        long parsed = strtol(pArgs, &rem, 10);
        if (rem[strspn(rem, " \t")] == '\0' && parsed > 0 && parsed <= PLANET_NR) {
            const Uns16 planetId = (Uns16) parsed;
            if (pRace == State_PlanetOwner(pInfo->pState, planetId)) {
                State_SetBuildRequest(pInfo->pState, planetId, True);
            } else {
                Info("\t(-) rejected command from %d: '%s' (not owned)", (int) pRace, pWholeLine);
                Message_CactusFailed_NotOwned(pRace, planetId);
            }
        } else {
            Info("\t(-) rejected command from %d: '%s' (syntax error)", (int) pRace, pWholeLine);
            Message_CommandSyntaxError(pRace, pWholeLine);
        }
        return True;
    } else if (strcasecmp(pCommand, "vote") == 0) {
        // Vote for end
        if (strcasecmp(pArgs, "yes") == 0 || strcasecmp(pArgs, "true") == 0) {
            State_SetVote(pInfo->pState, pRace, True);
        } else if (strcasecmp(pArgs, "no") == 0 || strcasecmp(pArgs, "false") == 0) {
            State_SetVote(pInfo->pState, pRace, False);
        } else {
            Info("\t(-) rejected command from %d: '%s' (syntax error)", (int) pRace, pWholeLine);
            Message_CommandSyntaxError(pRace, pWholeLine);
        }
        return True;
    } else {
        return False;
    }
}

void ProcessCommands(struct State* pState, const struct Config* pConfig)
{
    struct CommandInfo info;
    info.pState = pState;
    info.pConfig = pConfig;

    Info("    Checking commands...");
    CommandFileReader(0 /* all players */,
                      CheckCommand,
                      0 /* CommandComplain_Func */,
                      "cactus",
                      0 /* pPrivateFile */,
                      &info);
}
