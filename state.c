/**
  *  \file state.c
  *  \brief Agave Tequilana - Status
  */

#include "state.h"
#include "util.h"

static const char*const STATE_FILE_NAME = "cactus.hst";

/** Race Array: per-player integers.
    (I generally like to use the term 'player' instead of 'race',
    but 'race' has a better hamming distance to 'planet' here.) */
struct RaceArray {
    Int16 Values[RACE_NR];           ///< Value (score, count, etc.).
};

/** Planet Array: per-planet integers.
    Used to store owners or flags, thus, a char is sufficient. */
struct PlanetArray {
    char Values[PLANET_NR];          ///< Value (owner, flag).
};

/** Overall State.
    Consists of persistent state that is stored in the `cactus.hst` file, and transient state.
    For compatiblity with the original implementation,
    some information that can be regenerated at any time is also stored as persistent state.

    This class is private, use State_Create() to create it, State_Destroy() to destroy it,
    and the other State_XXX functions to access it.
    All functions deal gracefully with out-of-range parameters, that is, ignore the call or return 0. */
struct State {
    /*
     *  Persistent state
     */

    /** For each planet, nonzero if there is a full cactus. */
    struct PlanetArray HasFullCactus;

    /** For each planet, last owner. Used to detect ownership changes. */
    struct PlanetArray LastPlanetOwner;

    /** For each planet, builder of the cactus. */
    struct PlanetArray CactusBuilder;

    /** For each player, score. */
    struct RaceArray Score;

    /** For each player, number of owned cactuses (or stumps).
        (Can be regenerated at any time.) */
    struct RaceArray NumOwnedCactuses;

    /** For each player, number of built cactuses, independant of owner.
        (Can be regenerated at any time.) */
    struct RaceArray NumBuiltCactuses;

    /** For each player, vote status. */
    struct RaceArray VoteStatus;

    /*
     *  Transient state
     */

    /** For each planet, nonzero to build a cactus. */
    struct PlanetArray BuildRequest;

    /** For each player, number of cactuses built this turn. */
    struct RaceArray NumCactusesBuiltThisTurn;

    /** Previous scores, for difference reporting. */
    struct RaceArray OldScore;

    /** Previous cactus counts, for difference reporting. */
    struct RaceArray OldNumOwnedCactuses;

    /** Overall "is-finished" state. */
    Boolean IsFinished;
};


/*
 *  RaceArray methods
 */

static Int16 RaceArray_Get(const struct RaceArray* p, RaceType_Def race)
{
    return (race > 0 && race <= RACE_NR
            ? p->Values[race-1]
            : 0);
}

static void RaceArray_Set(struct RaceArray* p, RaceType_Def race, Int16 value)
{
    if (race > 0 && race <= RACE_NR) {
        p->Values[race-1] = value;
    }
}

static void RaceArray_Add(struct RaceArray* p, RaceType_Def race, int value)
{
    Int32 newValue = (Int32) RaceArray_Get(p, race) + value;
    RaceArray_Set(p, race, (Int16) MAX(-32768, MIN(32767, newValue)));
}

static void RaceArray_Clear(struct RaceArray* p)
{
    for (int i = 0; i < RACE_NR; ++i) {
        p->Values[i] = 0;
    }
}

static Boolean RaceArray_Load(struct RaceArray* p, FILE* fp)
{
    return DOSRead16((Uns16*) p->Values, RACE_NR, fp);
}

static Boolean RaceArray_LoadBytes(struct RaceArray* p, FILE* fp)
{
    char tmp[RACE_NR];
    if (fread(tmp, 1, RACE_NR, fp) == RACE_NR) {
        for (int i = 0; i < RACE_NR; ++i) {
            p->Values[i] = tmp[i];
        }
        return True;
    } else {
        return False;
    }
}

static Boolean RaceArray_Save(const struct RaceArray* p, FILE* fp)
{
    return DOSWrite16((const Uns16*) p->Values, RACE_NR, fp);
}

static Boolean RaceArray_SaveBytes(const struct RaceArray* p, FILE* fp)
{
    char tmp[RACE_NR];
    for (int i = 0; i < RACE_NR; ++i) {
        tmp[i] = (char) p->Values[i];
    }
    return (fwrite(tmp, 1, RACE_NR, fp) == RACE_NR);
}


/*
 *  PlanetArray methods
 */

static Int16 PlanetArray_Get(const struct PlanetArray* p, Uns16 planet)
{
    return (planet > 0 && planet <= PLANET_NR
            ? p->Values[planet-1]
            : 0);
}

static void PlanetArray_Set(struct PlanetArray* p, Uns16 planet, int value)
{
    if (planet > 0 && planet <= PLANET_NR) {
        p->Values[planet-1] = (char)value;
    }
}

static void PlanetArray_Clear(struct PlanetArray* p)
{
    for (int i = 0; i < PLANET_NR; ++i) {
        p->Values[i] = 0;
    }
}

static Boolean PlanetArray_Load(struct PlanetArray* p, FILE* fp)
{
    return fread(p->Values, 1, PLANET_NR, fp) == PLANET_NR;
}

static Boolean PlanetArray_Save(const struct PlanetArray* p, FILE* fp)
{
    return fwrite(p->Values, 1, PLANET_NR, fp) == PLANET_NR;
}


/*
 *  State Methods
 */

struct State* State_Create()
{
    struct State* p = MemAlloc(sizeof(struct State));
    State_Reset(p, False);
    return p;
}

void State_Destroy(struct State* pState)
{
    MemFree(pState);
}

void State_Load(struct State* pState, Boolean initOwners)
{
    State_Reset(pState, initOwners);

    FILE* fp = OpenInputFile(STATE_FILE_NAME, GAME_DIR_ONLY | NO_MISSING_ERROR);
    if (fp != 0) {
        Uns16 turn;
        Boolean ok = PlanetArray_Load(&pState->HasFullCactus, fp)
            && PlanetArray_Load(&pState->LastPlanetOwner, fp)
            && RaceArray_Load(&pState->Score, fp)
            && RaceArray_Load(&pState->NumOwnedCactuses, fp)
            && RaceArray_LoadBytes(&pState->VoteStatus, fp)
            && DOSRead16(&turn, 1, fp)
            && PlanetArray_Load(&pState->CactusBuilder, fp)
            && RaceArray_Load(&pState->NumBuiltCactuses, fp);
        fclose(fp);

        if (ok) {
            pState->OldScore = pState->Score;
            pState->OldNumOwnedCactuses = pState->NumOwnedCactuses;
        } else {
            Error("Unable to read state file; discarding state");
            State_Reset(pState, initOwners);
        }
    }
}

void State_Reset(struct State* pState, Boolean initOwners)
{
    // Persistent state
    PlanetArray_Clear(&pState->HasFullCactus);
    PlanetArray_Clear(&pState->LastPlanetOwner);
    PlanetArray_Clear(&pState->CactusBuilder);
    RaceArray_Clear(&pState->Score);
    RaceArray_Clear(&pState->NumOwnedCactuses);
    RaceArray_Clear(&pState->NumBuiltCactuses);
    RaceArray_Clear(&pState->VoteStatus);

    // Transient state
    PlanetArray_Clear(&pState->BuildRequest);
    RaceArray_Clear(&pState->NumCactusesBuiltThisTurn);
    RaceArray_Clear(&pState->OldScore);
    RaceArray_Clear(&pState->OldNumOwnedCactuses);
    pState->IsFinished = False;

    // Initialize LastPlanetOwner.
    // This mainly helps the system tests because it allows building a cactus on the first run;
    // otherwise we'd have to run one extra host.
    // It shouldn't affect regular game operation.
    if (initOwners) {
        for (Uns16 i = 1; i <= PLANET_NR; ++i) {
            PlanetArray_Set(&pState->LastPlanetOwner, i, PlanetOwner(i));
        }
    }
}

void State_Save(const struct State* pState)
{
    // Currently, OpenOutputFile will always ErrorExit on error.
    FILE* fp = OpenOutputFile(STATE_FILE_NAME, GAME_DIR_ONLY | NO_MISSING_ERROR);
    Boolean ok = False;
    if (fp != 0) {
        const Uns16 turn = TurnNumber();
        ok = PlanetArray_Save(&pState->HasFullCactus, fp)
            && PlanetArray_Save(&pState->LastPlanetOwner, fp)
            && RaceArray_Save(&pState->Score, fp)
            && RaceArray_Save(&pState->NumOwnedCactuses, fp)
            && RaceArray_SaveBytes(&pState->VoteStatus, fp)
            && DOSWrite16(&turn, 1, fp)
            && PlanetArray_Save(&pState->CactusBuilder, fp)
            && RaceArray_Save(&pState->NumBuiltCactuses, fp);
        fclose(fp);
    }
    if (!ok) {
        Error("Unable to write state file; state has been lost");
    }
}

void State_Dump(const struct State* pState, FILE* fp)
{
    // Scores
    fprintf(fp,
            "Player    Score    Built    Owned\n"
            "-------  -------  -------  -------\n");
    for (int i = 1; i <= RACE_NR; ++i) {
        fprintf(fp, "%5d  %7d %7d %7d\n", i,
                RaceArray_Get(&pState->Score, i),
                RaceArray_Get(&pState->NumBuiltCactuses, i),
                RaceArray_Get(&pState->NumOwnedCactuses, i));
    }

    // Planets
    fprintf(fp,
            "\n"
            "Planet    Owner   Builder   Type\n"
            "-------  -------  -------  -------\n");
    for (int i = 1; i <= PLANET_NR; ++i) {
        if (State_PlanetHasCactus(pState, i)) {
            fprintf(fp, "%5d  %7d %7d     %s\n", i,
                    PlanetArray_Get(&pState->LastPlanetOwner, i),
                    PlanetArray_Get(&pState->CactusBuilder, i),
                    State_PlanetHasFullCactus(pState, i) ? "cactus" : "stump");
        }
    }
}

void State_UpdateCounts(struct State* pState)
{
    RaceArray_Clear(&pState->NumOwnedCactuses);
    RaceArray_Clear(&pState->NumBuiltCactuses);
    for (Uns16 planetId = 1; planetId <= PLANET_NR; ++planetId) {
        if (State_PlanetHasCactus(pState, planetId)) {
            RaceArray_Add(&pState->NumOwnedCactuses, PlanetOwner(planetId), 1);
            RaceArray_Add(&pState->NumBuiltCactuses, State_CactusBuilder(pState, planetId), 1);
        }
    }
}

/*
 *  Cactus Status
 */

Boolean State_PlanetHasCactus(const struct State* pState, Uns16 planetId)
{
    return PlanetArray_Get(&pState->CactusBuilder, planetId) != 0;
}

Boolean State_PlanetHasFullCactus(const struct State* pState, Uns16 planetId)
{
    return State_PlanetHasCactus(pState, planetId)
        && PlanetArray_Get(&pState->HasFullCactus, planetId) != 0;
}

RaceType_Def State_CactusBuilder(const struct State* pState, Uns16 planetId)
{
    return PlanetArray_Get(&pState->CactusBuilder, planetId);
}

void State_RemoveCactus(struct State* pState, Uns16 planetId, Boolean keepStump)
{
    RaceType_Def builder = PlanetArray_Get(&pState->CactusBuilder, planetId);
    if (builder != NoRace) {
        PlanetArray_Set(&pState->HasFullCactus, planetId, 0);
        if (!keepStump) {
            RaceArray_Add(&pState->NumBuiltCactuses, builder, -1);
            RaceArray_Add(&pState->NumOwnedCactuses, PlanetArray_Get(&pState->LastPlanetOwner, planetId), -1);
            PlanetArray_Set(&pState->CactusBuilder, planetId, 0);
        }
    }
}

void State_CreateCactus(struct State* pState, Uns16 planetId, RaceType_Def owner)
{
    State_RemoveCactus(pState, planetId, False);

    PlanetArray_Set(&pState->HasFullCactus, planetId, owner);
    PlanetArray_Set(&pState->CactusBuilder, planetId, owner);
    RaceArray_Add(&pState->NumBuiltCactuses, owner, +1);
    RaceArray_Add(&pState->NumOwnedCactuses, owner, +1);
    RaceArray_Add(&pState->NumCactusesBuiltThisTurn, owner, +1);
}

int State_NumBuiltCactuses(const struct State* pState, RaceType_Def owner)
{
    return RaceArray_Get(&pState->NumBuiltCactuses, owner);
}

int State_NumOwnedCactuses(const struct State* pState, RaceType_Def owner)
{
    return RaceArray_Get(&pState->NumOwnedCactuses, owner);
}

int State_NumCactusesBuiltThisTurn(const struct State* pState, RaceType_Def owner)
{
    return RaceArray_Get(&pState->NumCactusesBuiltThisTurn, owner);
}

int State_NumOwnedCactusesChange(const struct State* pState, RaceType_Def owner)
{
    return RaceArray_Get(&pState->NumOwnedCactuses, owner)
        - RaceArray_Get(&pState->OldNumOwnedCactuses, owner);
}

/*
 *  Planet Ownership
 */

RaceType_Def State_PlanetOwner(const struct State* pState, Uns16 planetId)
{
    return PlanetArray_Get(&pState->LastPlanetOwner, planetId);
}

void State_SetPlanetOwner(struct State* pState, Uns16 planetId, RaceType_Def newOwner)
{
    if (State_PlanetHasCactus(pState, planetId)) {
        RaceType_Def oldOwner = PlanetArray_Get(&pState->LastPlanetOwner, planetId);
        RaceArray_Add(&pState->NumOwnedCactuses, oldOwner, -1);
        RaceArray_Add(&pState->NumOwnedCactuses, newOwner, +1);
    }
    PlanetArray_Set(&pState->LastPlanetOwner, planetId, (Int16) newOwner);
}

int State_CountPlanets(const struct State* pState, RaceType_Def race)
{
    int result = 0;
    for (Uns16 i = 1; i <= PLANET_NR; ++i) {
        if (PlanetArray_Get(&pState->LastPlanetOwner, i) == (Int16)race) {
            ++result;
        }
    }
    return result;
}


/*
 *  Build Request
 */

Boolean State_HasBuildRequest(const struct State* pState, Uns16 planetId)
{
    return PlanetArray_Get(&pState->BuildRequest, planetId);
}

void State_SetBuildRequest(struct State* pState, Uns16 planetId, Boolean flag)
{
    PlanetArray_Set(&pState->BuildRequest, planetId, flag);
}

/*
 *  Voting
 */

Boolean State_HasVote(const struct State* pState, RaceType_Def race)
{
    return RaceArray_Get(&pState->VoteStatus, race);
}

void State_SetVote(struct State* pState, RaceType_Def race, Boolean flag)
{
    RaceArray_Set(&pState->VoteStatus, race, flag);
}

Boolean State_IsFinished(const struct State* pState)
{
    return pState->IsFinished;
}

void State_SetFinished(struct State* pState, Boolean flag)
{
    pState->IsFinished = flag;
}

/*
 *  Score
 */

void State_AddScore(struct State* pState, RaceType_Def race, int delta)
{
    RaceArray_Add(&pState->Score, race, delta);
    Info("\t    player %d, score %d => %d", (int)race, (int)delta, RaceArray_Get(&pState->Score, race));
}

Int16 State_Score(const struct State* pState, RaceType_Def race)
{
    return RaceArray_Get(&pState->Score, race);
}

int State_ScoreChange(const struct State* pState, RaceType_Def race)
{
    return RaceArray_Get(&pState->Score, race)
        - RaceArray_Get(&pState->OldScore, race);
}
