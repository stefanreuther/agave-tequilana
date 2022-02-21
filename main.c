/**
  *  \file main.c
  *  \brief Agave Tequilana - Main Entry Point
  *
  *  \mainpage
  *
  *  Agave Tequilana is an add-on for VGA Planets, modeled after the
  *  Cactus/TequilaWar add-on. It implements a scoring system that
  *  requires players to capture and keep planets.
  */

#include <phostpdk.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "config.h"
#include "score.h"
#include "sendconf.h"
#include "state.h"
#include "version.h"

static const char*const BANNER = "Agave Tequilana - A Tequila War Variant";
static const char*const LOG_FILE = "cactus.log";

enum Mode {
    HostAction,
    DumpStatus,
    DumpConfig,
    Help
};

static void PrintUsage(FILE* stream, const char* name)
{
    fprintf(stream, "%s - v%s\n\n"
            "Usage: %s [MODE] [GAMEDIR [ROOTDIR]]\n\n"
            "MODE is:\n"
            "  -dc     dump config\n"
            "  -ds     dump status\n"
            "  -i      c2host integration (c2ref.txt, c2score.txt)\n"
            "  --help  this message\n\n"
            "Written in 2021 by Stefan Reuther <streu@gmx.de> for PlanetsCentral\n"
            "Based upon Cactus 2001++ by K.Kopytov, E.Goroh\n",
            BANNER, VERSION, name);
}

static void InitHostAction(struct Config* c)
{
    InitPHOSTLib();
    gLogFile = OpenOutputFile(LOG_FILE, GAME_DIR_ONLY | TEXT_MODE);
    Info("Loading...");
    if (!ReadGlobalData()) {
        FreePHOSTLib();
        ErrorExit("Unable to read global data");
    }
    if (!ReadHostData()) {
        FreePHOSTLib();
        ErrorExit("Unable to read host data");
    }
    Config_Load(c);

    // Set util.tmp mode. This causes our util.dat records come out in the right order.
    // In particular, our mine scans come out before PHost's.
    SetUtilMode(UTIL_Tmp);
}

static void DoneHostAction()
{
    Info("Saving...");
    if (!WriteHostData()) {
        FreePHOSTLib();
        ErrorExit("Unable to write host data");
    }
    FreePHOSTLib();
}

/*
 *  HostAction mode
 */

static void DoHostAction(Boolean integrate)
{
    struct Config c;
    InitHostAction(&c);

    Info("Agave Tequilana v%s", VERSION);
    struct State* pState = State_Create();
    State_Load(pState, True);
    State_UpdateCounts(pState);

    DoSendConfig(&c);
    ProcessCommands(pState, &c);
    if (c.ProcessMessages) {
        ProcessMessages(pState, &c);
    }
    ProcessBuildRequests(pState, &c);
    ComputeScores(pState, &c);
    ProcessVotes(pState, &c, integrate);
    SendReports(pState, &c);
    if (integrate) {
        SaveScoreFile(pState);
    }

    State_Save(pState);
    State_Destroy(pState);

    DoneHostAction();
}

/*
 *  DumpConfig Mode
 */

static void DumpConfig_Show(void* state, const char* name, const char* value)
{
    (void) state;
    Info("%s = %s", name, value);
}

static void DoDumpConfig()
{
    struct Config c;
    InitPHOSTLib();
    Config_Load(&c);
    Config_Format(&c, DumpConfig_Show, NULL);
    FreePHOSTLib();
}

/*
 *  DumpStatus mode
 */

static void DoDumpStatus()
{
    struct State* pState = State_Create();
    State_Load(pState, False);
    State_Dump(pState, stdout);
    State_Destroy(pState);
}

/*
 *  Main Entry Point
 */

int main(int argc, char** argv)
{
    (void) argc;
    enum Mode mode = HostAction;

    // Parse command line
    int i = 1;
    Boolean hasGame = False, hasRoot = False;
    Boolean integrate = False;
    while (argv[i] != 0) {
        const char* p = argv[i];
        if (*p == '-') {
            // Options
            while (*p == '-') {
                ++p;
            }
            if (strcmp(p, "dc") == 0) {
                mode = DumpConfig;
            } else if (strcmp(p, "ds") == 0) {
                mode = DumpStatus;
            } else if (strcmp(p, "i") == 0) {
                integrate = True;
            } else if (strcmp(p, "help") == 0 || strcmp(p, "h") == 0) {
                mode = Help;
            } else {
                PrintUsage(stderr, argv[0]);
                return 1;
            }
        } else if (!hasGame) {
            // Game directory
            gGameDirectory = p;
            hasGame = True;
        } else if (!hasRoot) {
            // Root directory
            gRootDirectory = p;
            hasRoot = True;
        } else {
            // Excess non-option
            PrintUsage(stderr, argv[0]);
            return 1;
        }
        ++i;
    }

    if ((mode != Help && mode != DumpConfig) && !hasGame) {
        // No command-line option at all
        PrintUsage(stderr, argv[0]);
        return 1;
    }

    switch (mode) {
     case HostAction:
        DoHostAction(integrate);
        break;
     case DumpConfig:
        DoDumpConfig();
        break;
     case DumpStatus:
        DoDumpStatus();
        break;
     case Help:
        PrintUsage(stdout, argv[0]);
        break;
    }
    return 0;
}
