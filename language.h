/**
  *  \file language.h
  *  \brief Agave Tequilana - Language
  */
#ifndef LANGUAGE_H_INCLUDED
#define LANGUAGE_H_INCLUDED

#include <phostpdk.h>

/** Definition for a single language.
    Each string is (part of) a message template. */
struct Language {
    // Error
    const char* Message_CommandSyntaxError_Top;            ///< "Command...."
    const char* Message_CommandSyntaxError_Bottom;         ///< "...had a syntax error."

    // Reports
    const char* Message_ScoreReport;                       ///< "Here's your score report:".
    const char* Message_InventoryReport_Header;            ///< "Here's your inventory report:".
    const char* Message_InventoryReport_Continuation;      ///< "Continuation of your inventory report:".
    const char* ReportScores_Header;                       ///< Header of score table.
    const char* ReportScores_Footer;                       ///< Footer off score table.

    // Ownership change
    const char* Message_CactusCaptured_Previous;           ///< "Your cactus was captured."
    const char* Message_CactusCaptured_Current;            ///< "You captured a cactus."
    const char* Message_CactusLost;                        ///< "You lost a cactus."

    // Cactus building
    const char* Message_CactusBuilt;                       ///< "You built a cactus."
    const char* Message_CactusFailed_NotOwned;             ///< "You do not own this planet."
    const char* Message_CactusFailed_HasFullCactus;        ///< "You already have a cactus."
    const char* Message_CactusFailed_CannotRebuild;        ///< "Cannot build over a stump."
    const char* Message_CactusFailed_NeedBase;             ///< "You need a base."
    const char* Message_CactusFailed_ClansRequired;        ///< "You need clans."
    const char* Message_CactusFailed_CactusLimit;          ///< "You got too many cactuses."
    const char* Message_CactusFailed_MinScore;             ///< "You have too low score."

    // Voting
    const char* Message_VoteIgnored_Turn;                  ///< "You voted too early."
    const char* Message_VoteIgnored_Build;                 ///< "You cannot vote and build."

    // Configuration
    const char* SendConfig_Header;                         ///< "Here's the configuration:"
    const char* SendConfig_Continuation;                   ///< "Configuration continues..."

    const char* Continuation;                              ///< "...to be continued".

    // Scores
    const char* Score_NumOwnedCactuses;                    ///< Name of "Owned Cactuses" score.
    const char* Score_NumBuiltCactuses;                    ///< Name of "Built Cactuses" score.
    const char* Score_Score;                               ///< Name of overall score.
};

/** Get language for a player.
    Will never return null; if player has no (recognized) language, returns English.
    \param player Player */
const struct Language* GetLanguageForPlayer(RaceType_Def player);

#endif
