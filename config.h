/**
  *  \file config.h
  *  \brief Agave Tequilana - Configuration Handling
  */
#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <phostpdk.h>

/** Configuration structure.
    Member names match the names in the configuration file. */
struct Config {
    // General
    Boolean KeepCactus;
    Boolean ProcessMessages;

    // Scoring
    Int16 TurnScore;
    Int16 TurnOwnerScore;
    Int16 TurnPlusScore;
    Int16 TurnMinusScore;
    Int16 CaptureScore;
    Int16 LostScore;
    Int16 DeadScore;

    // Building
    Boolean NeedBase;
    Boolean RebuildCactus;
    Int16 ClansRequired;
    Int16 CactusLimit;
    Int16 CostAdditive;
    Int16 CostMultiplier;
    Int16 CostPower;
    Int16 MinScore;

    // Voting
    Boolean EnableFinish;
    Int16 VoteTurn;
    Int16 FinishPercent;
    Int16 FinishScore;
};

/** Initialize configuration.
    @param [out] p Configuration structure; will be set to defaults. */
void Config_Init(struct Config* p);

/** Load configuration.
    @param [out] p Configuration structure; will be set with loaded values.
    @pre PDK initialized (gGameDirectory set) */
void Config_Load(struct Config* p);

/** Format configuration.
    Calls the provided callback function for each configuration key,
    passing it the name and stringified value.
    This can be used for printing or sending messages.
    @param [in] p     Configuration
    @param [in] func  Callback function
    @param [in] state Opaque state pointer that is passed to the callback function */
void Config_Format(const struct Config* p, void func(void* state, const char* name, const char* value), void* state);

#endif
