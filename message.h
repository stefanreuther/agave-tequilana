/**
  *  \file message.h
  *  \brief Agave Tequilana- Messages
  */
#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#include <phostpdk.h>

/** Maxium message length in bytes. */
#define MAX_MESSAGE_LENGTH 600

/** Estimated maximum number of message lines.
    This is not a hard limit; code generating variable-length messages
    uses this as a guideline when to start a new message. */
#define MAX_MESSAGE_LINES 17


/*
 *  Low-Level Functions
 */

/** Message building state.
    Tracks content and size for a message. */
struct Message {
    size_t Length;                        /**< Number of characters so far. */
    size_t Lines;                         /**< Number of lines so far. */
    char Content[MAX_MESSAGE_LENGTH];     /**< Character buffer. */
};

/** Initialize a message.
    @param [out] m Structure to initialize */
void Message_Init(struct Message* m);

/** Add character to e message.
    Counts lines and characters.
    If the character limit is exceeded, excess characters are discarded.
    @param [in,out] m  Message
    @param [in]     ch Character to add */
void Message_AddChar(struct Message* m, char ch);

/** Add string to a message.
    Also see Message_AddChar.
    @param [in,out] m   Message
    @param [in]     str String to add */
void Message_Add(struct Message* m, const char* str);

/** Add formatted text to a message.
    Processes a message template, interpolating placeholders.
    Each placeholder has the format `%<index><type>`,
    where `<index>` is the 0-based index into @c args,
    and `<type>` is one of the following:
    - '%' single percent sign
    - 'I' integer formatted with leading zeroes (%04d)
    - 'd' integer
    - 'B' boolean (yes/no)
    - 'A' race name adjective
    - 'P' planet name
    - 'S' ship name
    - 'R' race short name
    - 'V' version number

    Also see Message_AddChar.

    @param [in,out] m       Message
    @param [in]     tpl     Message template
    @param [in]     args    Parameters
    @param [in]     numArgs Number of parameters (number of elements in args) */
void Message_Format(struct Message* m, const char* tpl, const Int32* args, size_t numArgs);

/** Send message.
    @param [in] m  Message
    @param [in] to Player to receive the message */
void Message_Send(struct Message* m, RaceType_Def to);


/*
 *  Higher-Level Functions
 */

/** Send message with template.
    This combines the Message_Init, Message_Format, Message_Send functions in one call.
    @param [in] to      Receiver
    @param [in] tpl     Template; see Message_Format
    @param [in] args    Parameters; ses Message_Format
    @param [in] numArgs Number of parameters; see Message_Format */
void Message_SendTemplate(RaceType_Def to, const char* tpl, const Int32* args, size_t numArgs);

/*
 *  Canned Messages
 */

/** Report a command syntax error.
    @param to  Player
    @param cmd Failing command */
void Message_CommandSyntaxError(RaceType_Def to, const char* cmd);

/** Score report.
    @param to  Player
    @param numOwnedCactuses  Number of cactuses owned
    @param numBuiltCactuses  Number of cactuses built
    @param score             Current score
    @param vote              Vote status
    @param numVotes          Number of votes this player has */
void Message_ScoreReport(RaceType_Def to, int numOwnedCactuses, int numBuiltCactuses, int score, Boolean vote, int numVotes);

/** Report captured cactus.
    Sends messages to both captor and captee.
    @param prev       Previous owner (captee)
    @param curr       Current owner (captor)
    @param planetId   Planet Id
    @param prevScore  Score awarded to previous owner
    @param currScore  Score awarded to current owner */
void Message_CactusCaptured(RaceType_Def prev, RaceType_Def curr, Uns16 planetId, int prevScore, int currScore);

/** Report lost cactus.
    @param prev       Previous owner (captee), receives the message
    @param planetId   Planet Id
    @param prevScore  Score awarded to previous owner */
void Message_CactusLost(RaceType_Def prev, Uns16 planetId, int prevScore);

/** Report cactus successfully built.
    @param to        Player
    @param planetId  Planet Id
    @param cost      Cost of cactus */
void Message_CactusBuilt(RaceType_Def to, Uns16 planetId, int cost);

/** Report failure to build cactus: planet not owned.
    @param to        Player
    @param planetId  Planet Id */
void Message_CactusFailed_NotOwned(RaceType_Def to, Uns16 planetId);

/** Report failure to build cactus: full cactus already present.
    @param to        Player
    @param planetId  Planet Id */
void Message_CactusFailed_HasFullCactus(RaceType_Def to, Uns16 planetId);

/** Report failure to build cactus: stump present but RebuildCactus=No.
    @param to        Player
    @param planetId  Planet Id */
void Message_CactusFailed_CannotRebuild(RaceType_Def to, Uns16 planetId);

/** Report failure to build cactus: starbase required.
    @param to        Player
    @param planetId  Planet Id */
void Message_CactusFailed_NeedBase(RaceType_Def to, Uns16 planetId);

/** Report failure to build cactus: minimum clans not present.
    @param to            Player
    @param planetId      Planet Id
    @param clansRequired Number of clans required */
void Message_CactusFailed_ClansRequired(RaceType_Def to, Uns16 planetId, int clansRequired);

/** Report failure to build cactus: cactus limit exceeded.
    @param to          Player
    @param planetId    Planet Id
    @param cactusLimit Limit*/
void Message_CactusFailed_CactusLimit(RaceType_Def to, Uns16 planetId, int cactusLimit);

/** Report failure to build cactus: below minimum score.
    @param to        Player
    @param planetId  Planet Id */
void Message_CactusFailed_MinScore(RaceType_Def to, Uns16 planetId);

/** Report ignored vote: minimum turn not reached.
    @param to Player */
void Message_VoteIgnored_Turn(RaceType_Def to);

/** Report ignored vote: cannot vote when building cactus.
    @param to Player */
void Message_VoteIgnored_Build(RaceType_Def to);

#endif
