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
    Boolean KeepCactus;                 ///< True to support cactus stumps.
    Boolean ProcessMessages;            ///< True to process messages; false to process only commands.

    // Scoring
    Int16 TurnScore;                    ///< Points per turn for normal cactus.
    Int16 TurnOwnerScore;               ///< Points per turn for stump that you built.
    Int16 TurnPlusScore;                ///< Points per turn for captured stump.
    Int16 TurnMinusScore;               ///< Points per turn for lost stump.
    Int16 CaptureScore;                 ///< Points per turn for capturing a cactus.
    Int16 LostScore;                    ///< Points per turn for losing a cactus to someone else.
    Int16 DeadScore;                    ///< Points per turn for losing a cactus.

    // Building
    Boolean NeedBase;                   ///< True if building a cactus needs a base.
    Boolean RebuildCactus;              ///< True to allow building over a stump.
    Int16 ClansRequired;                ///< Clans required to build a cactus.
    Int16 CactusLimit;                  ///< Maximum number of cactuses per player.
    Int16 CostAdditive;                 ///< Flat score cost per cactus.
    Int16 CostMultiplier;               ///< Multiplicative score cost per cactus.
    Int16 CostPower;                    ///< Exponential score cost per cactus.
    Int16 MinScore;                     ///< Minimum score required to build cactus.

    // Voting
    Boolean EnableFinish;               ///< True to enable voting.
    Int16 VoteTurn;                     ///< Turn when to enable voting.
    Int16 FinishPercent;                ///< Percentage of votes that ends the game.
    Int16 FinishScore;                  ///< Game ends when player reaches this score.
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
