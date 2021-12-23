/**
  *  \file utildata.h
  *  \brief Agave Tequilana - util.data records
  */
#ifndef UTILDATA_H_INCLUDED
#define UTILDATA_H_INCLUDED

#include <phostpdk.h>

/** Score Id for Util_PlayerScore: number of owned cactuses. */
static const Uns16 SCORE_NUM_OWNED_CACTUSES = 0x40A0;

/** Score Id for Util_PlayerScore: number of built cactuses. */
static const Uns16 SCORE_NUM_BUILT_CACTUSES = 0x40A1;

/** Score Id for Util_PlayerScore: score (clamped at 0). */
static const Uns16 SCORE_SCORE = 0x40A2;


/** Type of a cactus for Util_Cactus(). */
enum CactusType {
    Cactus_Full = 0,
    Cactus_Foreign = 1,
    Cactus_Exile = 2,
    Cactus_Stump = 3
};


/** Write a "Player Score" record.
    Each of these records reports one score summary for all players.

    @param to                Receiver
    @param name              Human-readable score name
    @param scoreId           Machine-readable score Id, one of the SCORE_XXX constants.
    @param winLimit          Win-limit, can be -1 for none.
    @param score             Scores, >=0 for valid values, -1 if unknown. */
void Util_PlayerScore(RaceType_Def to, const char* name, Uns16 scoreId, Int16 winLimit, Uns32 (*score)[RACE_NR]);

/** Write a "Score" custom record.
    One such record is sent to every player.

    @param to                Receiver
    @param numOwnedCactuses  Number of owned cactuses, see State_NumOwnedCactuses()
    @param numBuiltCactuses  Number of built cactuses, see State_NumBuiltCactuses()
    @param score             Score, see State_Score(). Can be negative.
    @param vote              Current vote status */
void Util_Score(RaceType_Def to, int numOwnedCactuses, int numBuiltCactuses, int score, Boolean vote);

/** Write a "Cactus" custom record.
    One such record is sent to every player owning or having built a cactus.

    @param to                Receiver
    @param planetId          Planet Id
    @param type              Cactus type */
void Util_Cactus(RaceType_Def to, Uns16 planetId, enum CactusType type);

#endif
