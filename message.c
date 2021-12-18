/**
  *  \file message.c
  *  \brief Agave Tequilana - Messages
  */

#include <assert.h>
#include <string.h>
#include "message.h"
#include "language.h"
#include "version.h"


void Message_Init(struct Message* m)
{
    m->Length = 0;
    m->Lines = 0;
}

void Message_AddChar(struct Message* m, char ch)
{
    if (m->Length < MAX_MESSAGE_LENGTH-1) {
        if (ch == '\n') {
            // Newline transmitted as \r (13) in VGAP
            ++m->Lines;
            m->Content[m->Length] = 13;
            ++m->Length;
        } else if ((unsigned char) ch > 255-13) {
            // These characters cannot be handled by ROT-13 encryption; discard.
            // They can possibly appear in user-provided names.
        } else {
            // Normal case
            m->Content[m->Length] = ch;
            ++m->Length;
        }
    }
}

void Message_Add(struct Message* m, const char* str)
{
    char ch;
    while ((ch = *str++) != '\0') {
        Message_AddChar(m, ch);
    }
}

void Message_Format(struct Message* m, const char* tpl, const Int32* args, size_t numArgs)
{
    char ch;
    while ((ch = *tpl++) != '\0') {
        if (ch == '%') {
            // Parameter index
            size_t index = 0;
            while (*tpl >= '0' && *tpl <= '9') {
                index = 10*index + (*tpl++ - '0');
            }

            // Format it
            char fmt = *tpl++;
            char tmp[100];
            switch (fmt) {
             case '%':
                Message_Add(m, "%");
                break;

             case 'I':
                // 4-digit Id
                sprintf(tmp, "%04ld", (long) (index < numArgs ? args[index] : 0));
                Message_Add(m, tmp);
                break;

             case 'd':
                // Normal decimal number
                sprintf(tmp, "%ld", (long) (index < numArgs ? args[index] : 0));
                Message_Add(m, tmp);
                break;

             case 'B':
                // Boolean
                if (index < numArgs) {
                    Message_Add(m, args[index] != 0 ? "yes" : "no");
                }
                break;

             case 'A':
                // Adjective
                if (index < numArgs) {
                    RaceNameAdjective(args[index], tmp);
                    Message_Add(m, tmp);
                }
                break;

             case 'P':
                // Planet name
                if (index < numArgs) {
                    PlanetName((Uns16)args[index], tmp);
                    Message_Add(m, tmp);
                }
                break;

             case 'S':
                // Ship name
                if (index < numArgs) {
                    ShipName((Uns16)args[index], tmp);
                    Message_Add(m, tmp);
                }
                break;

             case 'R':
                // Race short name
                if (index < numArgs) {
                    RaceShortName(args[index], tmp);
                    Message_Add(m, tmp);
                }
                break;

             case 'V':
                // Version
                Message_Add(m, VERSION);
                break;
            }
        } else {
            Message_AddChar(m, ch);
        }
    }
}

void Message_Send(struct Message* m, RaceType_Def to)
{
    assert(m->Length < sizeof(m->Content));
    m->Content[m->Length] = '\0';
    WriteAUXHOSTMessage(to, m->Content);
}

void Message_SendTemplate(RaceType_Def to, const char* tpl, const Int32* args, size_t numArgs)
{
    struct Message m;
    Message_Init(&m);
    Message_Format(&m, tpl, args, numArgs);
    Message_Send(&m, to);
}

/*
 *  Canned Messages
 */

void Message_CommandSyntaxError(RaceType_Def to, const char* cmd)
{
    const struct Language* lang = GetLanguageForPlayer(to);
    struct Message m;
    Message_Init(&m);
    Message_Add(&m, lang->Message_CommandSyntaxError_Top);
    Message_Add(&m, cmd);
    Message_Add(&m, lang->Message_CommandSyntaxError_Bottom);
    Message_Send(&m, to);
}

void Message_ScoreReport(RaceType_Def to, int numOwnedCactuses, int numBuiltCactuses, int score, Boolean vote, int numVotes)
{
    Int32 args[] = { numOwnedCactuses, numBuiltCactuses, score, vote, numVotes };
    Message_SendTemplate(to, GetLanguageForPlayer(to)->Message_ScoreReport, args, 5);
}

void Message_CactusCaptured(RaceType_Def prev, RaceType_Def curr, Uns16 planetId, int prevScore, int currScore)
{
    Int32 args[] = { planetId, prevScore, currScore, prev, curr };
    Message_SendTemplate(prev, GetLanguageForPlayer(prev)->Message_CactusCaptured_Previous, args, 5);
    Message_SendTemplate(curr, GetLanguageForPlayer(curr)->Message_CactusCaptured_Current, args, 5);
}

void Message_CactusLost(RaceType_Def prev, Uns16 planetId, int prevScore)
{
    Int32 args[] = { planetId, prevScore };
    Message_SendTemplate(prev, GetLanguageForPlayer(prev)->Message_CactusLost, args, 2);
}

void Message_CactusBuilt(RaceType_Def to, Uns16 planetId, int cost)
{
    Int32 args[] = { planetId, cost };
    Message_SendTemplate(to, GetLanguageForPlayer(to)->Message_CactusBuilt, args, 2);
}

void Message_CactusFailed_NotOwned(RaceType_Def to, Uns16 planetId)
{
    Int32 args[] = { planetId };
    Message_SendTemplate(to, GetLanguageForPlayer(to)->Message_CactusFailed_NotOwned, args, 1);
}

void Message_CactusFailed_HasFullCactus(RaceType_Def to, Uns16 planetId)
{
    Int32 args[] = { planetId };
    Message_SendTemplate(to, GetLanguageForPlayer(to)->Message_CactusFailed_HasFullCactus, args, 1);
}

void Message_CactusFailed_CannotRebuild(RaceType_Def to, Uns16 planetId)
{
    Int32 args[] = { planetId };
    Message_SendTemplate(to, GetLanguageForPlayer(to)->Message_CactusFailed_CannotRebuild, args, 1);
}

void Message_CactusFailed_NeedBase(RaceType_Def to, Uns16 planetId)
{
    Int32 args[] = { planetId };
    Message_SendTemplate(to, GetLanguageForPlayer(to)->Message_CactusFailed_NeedBase, args, 1);
}

void Message_CactusFailed_ClansRequired(RaceType_Def to, Uns16 planetId, int clansRequired)
{
    Int32 args[] = { planetId, clansRequired };
    Message_SendTemplate(to, GetLanguageForPlayer(to)->Message_CactusFailed_ClansRequired, args, 2);
}

void Message_CactusFailed_CactusLimit(RaceType_Def to, Uns16 planetId, int cactusLimit)
{
    Int32 args[] = { planetId, cactusLimit };
    Message_SendTemplate(to, GetLanguageForPlayer(to)->Message_CactusFailed_CactusLimit, args, 2);
}

void Message_VoteIgnored_Turn(RaceType_Def to)
{
    Message_SendTemplate(to, GetLanguageForPlayer(to)->Message_VoteIgnored_Turn, 0, 0);
}

void Message_VoteIgnored_Build(RaceType_Def to)
{
    Message_SendTemplate(to, GetLanguageForPlayer(to)->Message_VoteIgnored_Build, 0, 0);
}
