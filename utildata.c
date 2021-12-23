/**
  *  \file utildata.c
  *  \brief Agave Tequilana - util.data records
  */

#include <stdio.h>
#include <string.h>
#include "utildata.h"
#include "version.h"

#define DIM(x) (sizeof(x)/sizeof(x[0]))

/* Player Score, standard record type */
static const Uns16 RECORD_PLAYER_SCORE = 51;

/* Agave Tequilana custom records */
static const Uns16 RECORD_SCORE = 0x40A0;
static const Uns16 RECORD_CACTUS = 0x40A1;

void Util_PlayerScore(RaceType_Def to, const char* name, Uns16 scoreId, Int16 winLimit, Uns32 (*score)[RACE_NR])
{
    char tag[50];
    strncpy(tag, name, sizeof(tag));

    Uns16 words[] = {
        scoreId,
        (Uns16) -1       // Turns-over-limit
    };
    WordSwapShort(words, DIM(words));

    Uns32 longs[RACE_NR+1];
    longs[0] = (Uns32) winLimit;
    for (int i = 0; i < RACE_NR; ++i) {
        longs[i+1] = (*score)[i];
    }
    WordSwapLong(longs, DIM(longs));

    void* pointers[] = { &tag, &words, &longs };
    Uns16 sizes[] = { sizeof(tag), sizeof(words), sizeof(longs) };
    PutUtilRecord(to, RECORD_PLAYER_SCORE, DIM(pointers), sizes, pointers);
}

void Util_Score(RaceType_Def to, int numOwnedCactuses, int numBuiltCactuses, int score, Boolean vote)
{
    char tag[32];
    memset(tag, 0, sizeof(tag));
    sprintf(tag, "Agave Tequilana %s", VERSION);

    Uns16 data[] = {
        (Uns16) numOwnedCactuses,
        (Uns16) numBuiltCactuses,
        (Uns16) score,
        (Uns16) vote
    };
    WordSwapShort(data, DIM(data));

    void* pointers[] = { &tag, &data };
    Uns16 sizes[] = { sizeof(tag), sizeof(data) };
    PutUtilRecord(to, RECORD_SCORE, DIM(pointers), sizes, pointers);
}


void Util_Cactus(RaceType_Def to, Uns16 planetId, enum CactusType type)
{
    Uns16 data[] = {
        planetId,
        (Uns16) type
    };

    WordSwapShort(data, DIM(data));
    PutUtilRecordSimple(to, RECORD_CACTUS, sizeof(data), &data);
}
