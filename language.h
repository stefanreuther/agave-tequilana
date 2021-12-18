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
    const char* Message_CommandSyntaxError_Top;
    const char* Message_CommandSyntaxError_Bottom;

    // Reports
    const char* Message_ScoreReport;
    const char* Message_InventoryReport_Header;
    const char* Message_InventoryReport_Continuation;
    const char* ReportScores_Header;
    const char* ReportScores_Footer;

    // Ownership change
    const char* Message_CactusCaptured_Previous;
    const char* Message_CactusCaptured_Current;
    const char* Message_CactusLost;

    // Cactus building
    const char* Message_CactusBuilt;
    const char* Message_CactusFailed_NotOwned;
    const char* Message_CactusFailed_HasFullCactus;
    const char* Message_CactusFailed_CannotRebuild;
    const char* Message_CactusFailed_NeedBase;
    const char* Message_CactusFailed_ClansRequired;
    const char* Message_CactusFailed_CactusLimit;

    // Voting
    const char* Message_VoteIgnored_Turn;
    const char* Message_VoteIgnored_Build;

    // Configuration
    const char* SendConfig_Header;
    const char* SendConfig_Continuation;

    const char* Continuation;
};

/** Get language for a player.
    Will never return null; if player has no (recognized) language, returns English.
    \param player Player */
const struct Language* GetLanguageForPlayer(RaceType_Def player);

#endif
