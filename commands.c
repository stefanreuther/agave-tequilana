/**
  *  \file commands.c
  *  \brief Agave Tequilana - Command Handling
  */

#include <strings.h>
#include <string.h>
#include <ctype.h>
#include "commands.h"
#include "message.h"
#include "util.h"

static const char MSG_TERMINATOR = 26;
static const char MSG_OFFSET = 13;

/** @private */
struct CommandInfo {
    struct State* pState;
    const struct Config* pConfig;
};

static void ProcessBuildCommand(Uns16 pRace, const char* pArgs, const char* pWholeLine, const struct CommandInfo*const pInfo)
{
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
}

static void ProcessVoteCommand(Uns16 pRace, const char* pArgs, const char* pWholeLine, const struct CommandInfo*const pInfo)
{
    if (strcasecmp(pArgs, "yes") == 0 || strcasecmp(pArgs, "true") == 0) {
        State_SetVote(pInfo->pState, pRace, True);
    } else if (strcasecmp(pArgs, "no") == 0 || strcasecmp(pArgs, "false") == 0) {
        State_SetVote(pInfo->pState, pRace, False);
    } else {
        Info("\t(-) rejected command from %d: '%s' (syntax error)", (int) pRace, pWholeLine);
        Message_CommandSyntaxError(pRace, pWholeLine);
    }
}

static Boolean CheckCommand(Uns16 pRace, const char* pCommand, const char* pArgs, const char* pWholeLine, void* pData)
{
    const struct CommandInfo*const pInfo = pData;

    if (strcasecmp(pCommand, "defhw") == 0 || strcasecmp(pCommand, "build") == 0) {
        // Plant a cactus
        ProcessBuildCommand(pRace, pArgs, pWholeLine, pInfo);
        return True;
    } else if (strcasecmp(pCommand, "vote") == 0) {
        // Vote for end
        ProcessVoteCommand(pRace, pArgs, pWholeLine, pInfo);
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

/*
 *  Classic/Legacy Message Processing
 */

static void CheckMessageLine(Uns16 pRace, const char* line, void* pData)
{
    const char* arg;
    if ((arg = StrStartsWith(line, "defhw ")) != 0 ||
        (arg = StrStartsWith(line, "build ")) != 0 ||
        (arg = StrStartsWith(line, "OBJECT: Planet ")) != 0)
    {
        ProcessBuildCommand(pRace, arg, line, pData);
    } else if ((arg = StrStartsWith(line, "vote ")) != 0) {
        ProcessVoteCommand(pRace, arg, line, pData);
    } else {
        // Ignore.
    }
}

static const char* ReadMessageLine(char** data, size_t* size)
{
    const char* result = *data;

    // Skip over line
    while (*size != 0 && **data != MSG_TERMINATOR) {
        **data -= MSG_OFFSET;
        ++*data;
        --*size;
    }

    // If we have data remaining, we're looking at a MSG_TERMINATOR.
    // Replace by null byte and return.
    if (*size != 0) {
        **data = '\0';
        ++*data;
        --*size;
        return result;
    } else {
        // Partial line: cannot return because there's no space for the null terminator
        // (or we ended the message already).
        return 0;
    }
}

/* Sort-of generic message file reader.
   Reads mess.tmp/messpnt.tmp, and forwards message-to-self to func(), line-by-line */
static void MessageFileReader(void func(Uns16, const char*, void*), void* pData)
{
    // Open files
    FILE* pointerFile = OpenInputFile("messpnt.tmp", GAME_DIR_ONLY | NO_MISSING_ERROR);
    FILE* dataFile = OpenInputFile("mess.tmp", GAME_DIR_ONLY | NO_MISSING_ERROR);

    // Process files
    if (pointerFile != 0 && dataFile != 0) {
        // Read count
        Uns16 numMessages;
        if (!DOSRead16(&numMessages, 1, pointerFile)) {
            numMessages = 0;
        }

        // Read messages
        for (Uns16 i = 0; i < numMessages; ++i) {
            // Single header
            enum { Receiver, AddressLo, AddressHi, Length };
            Uns16 header[4];
            if (!DOSRead16(header, 4, pointerFile)) {
                Warning("Unable to read header of message %d", (int) i);
                break;
            }

            // Message receiver
            Uns16 recv = header[Receiver];
            char recvChar = (recv > 0 && recv < 10 ? '0' + recv
                             : recv == 10 ? 'a'
                             : recv == 11 ? 'b'
                             : '\0');
            if (recvChar == '\0') {
                Warning("Invalid receiver for message %d", (int) i);
                break;
            }

            // Message position
            Uns32 pos = (Uns32)header[AddressLo] + (65536*header[AddressHi]);
            if (pos == 0) {
                Warning("Invalid position for message %d", (int) i);
                break;
            }
            if (fseek(dataFile, pos-1, SEEK_SET) != 0) {
                Warning("Unable to set position for message %d", (int) i);
                break;
            }

            // Message content
            char data[1000];
            size_t size = MIN(sizeof(data)-1, header[Length]);
            if (fread(data, 1, size, dataFile) != size) {
                Warning("Unable to read message %d", (int) i);
                break;
            }

            // Terminate it, so ReadMessageLine can extract lines
            data[size++] = MSG_TERMINATOR;

            // Read it
            char* p = data;
            const char* firstLine = ReadMessageLine(&p, &size);
            if (firstLine[0] == '('
                && firstLine[1] == '-'
                && firstLine[2] == 'r'
                && tolower((unsigned char) firstLine[3]) == recvChar)
            {
                // It's a message-to-self; parse it.
                const char* line;
                while ((line = ReadMessageLine(&p, &size)) != 0) {
                    func(recv, line, pData);
                }
            }
        }
    }

    // Close files
    if (pointerFile != 0) {
        fclose(pointerFile);
    }
    if (dataFile != 0) {
        fclose(dataFile);
    }
}

void ProcessMessages(struct State* pState, const struct Config* pConfig)
{
    struct CommandInfo info;
    info.pState = pState;
    info.pConfig = pConfig;

    Info("    Checking legacy commands...");
    MessageFileReader(CheckMessageLine, &info);
}
