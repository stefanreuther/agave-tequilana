/**
  *  \file state.h
  *  \brief Agave Tequilana - Status
  */
#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <phostpdk.h>
#include <stdio.h>

struct State;

/*
 *  General
 */

/** Create a new State structure.
    @return newly-allocated State. Never null. */
struct State* State_Create();

/** Discard a State structure.
    @param pState State. */
void State_Destroy(struct State* pState);

/** Load state.
    @param [out] pState     State. Will be loaded from game directory or initialized.
    @param [in]  initOwners True to initialize owner fields if there is no state file. */
void State_Load(struct State* pState, Boolean initOwners);

/** Reset state.
    @param [out] pState     State. Will be initialized to all-default.
    @param [in]  initOwners True to initialize owner fields if there is no state file. */
void State_Reset(struct State* pState, Boolean initOwners);

/** Save state.
    @param [in] pState State. Will be written to state file. */
void State_Save(const struct State* pState);

/** Dump state in human-readable form.
    @param [in] pState State
    @param [out] fp    Output file */
void State_Dump(const struct State* pState, FILE* fp);

/** Regenerate plant counts.
    The NumBuiltCactuses and NumOwnedCactuses fields can always be regenerated.
    This will fix a corrupted state.
    @param [in,out] pState State */
void State_UpdateCounts(struct State* pState);


/*
 *  Cactus Status
 */

/** Check whether planet has a cactus.
    @param [in]  pState    State
    @param [in]  planetId  Planet to check
    @return true if planet has a cactus or stump. */
Boolean State_PlanetHasCactus(const struct State* pState, Uns16 planetId);

/** Check whether planet has a full cactus.
    @param [in]  pState    State
    @param [in]  planetId  Planet to check
    @return true if planet has a cactus (not just a stump). */
Boolean State_PlanetHasFullCactus(const struct State* pState, Uns16 planetId);

/** Get builder of a cactus.
    @param [in]  pState    State
    @param [in]  planetId  Planet to check
    @return builder */
RaceType_Def State_CactusBuilder(const struct State* pState, Uns16 planetId);

/** Remove a cactus.
    Call is ignored if there is no cactus.
    @param [out] pState    State
    @param [in]  planetId  Planet to update
    @param [in]  keepStump True to keep a stump */
void State_RemoveCactus(struct State* pState, Uns16 planetId, Boolean keepStump);

/** Create a new cactus.
    @param [out] pState    State
    @param [in]  planetId  Planet to update
    @param [in]  owner     Cactus owner; should be same as planet owner */
void State_CreateCactus(struct State* pState, Uns16 planetId, RaceType_Def owner);

/** Get number of cactuses built by a player.
    This value updates automatically.
    @param [in]  pState    State
    @param [in]  owner     Player to check
    @return number of cactuses*/
int State_NumBuiltCactuses(const struct State* pState, RaceType_Def owner);

/** Get number of cactuses owned by a player.
    This value updates automatically.
    @param [in]  pState    State
    @param [in]  owner     Player to check
    @return number of cactuses */
int State_NumOwnedCactuses(const struct State* pState, RaceType_Def owner);

/** Get number of cactuses built this turn by a player (transient state).
    This value updates automatically.
    @param [in]  pState    State
    @param [in]  owner     Player to check
    @return number of cactuses */
int State_NumCactusesBuiltThisTurn(const struct State* pState, RaceType_Def owner);

/** Get change to number of cactuses owned by a player this turn (transient state).
    This value updates automatically.
    @param [in]  pState    State
    @param [in]  owner     Player to check
    @return change to number of cactuses */
int State_NumOwnedCactusesChange(const struct State* pState, RaceType_Def owner);


/*
 *  Planet Ownership
 */

/** Set planet owner.
    This is used to detect ownership change.
    @param [out]  pState   State
    @param [in]   planetId Planet Id
    @param [in]   owner    Planet owner */
void State_SetPlanetOwner(struct State* pState, Uns16 planetId, RaceType_Def owner);

/** Get planet owner.
    @param [in]   pState   State
    @param [in]   planetId Planet Id
    @return last owner */
RaceType_Def State_PlanetOwner(const struct State* pState, Uns16 planetId);

/** Count number of planets owned by a player.
    @param [in]   pState   State
    @param [in]   race     Player
    @return number */
int State_CountPlanets(const struct State* pState, RaceType_Def race);


/*
 *  Build Request
 */

/** Check for build request (transient state).
    @param [in]   pState   State
    @param [in]   planetId Planet Id
    @return true if build request was given */
Boolean State_HasBuildRequest(const struct State* pState, Uns16 planetId);

/** Set build request (transient state).
    @param [in]   pState   State
    @param [in]   planetId Planet Id
    @param [in]   flag     New state */
void State_SetBuildRequest(struct State* pState, Uns16 planetId, Boolean flag);


/*
 *  Voting/Finishing
 */

/** Check vote status.
    @param [in]   pState   State
    @param [in]   race     Player
    @return true if player voted to end the game */
Boolean State_HasVote(const struct State* pState, RaceType_Def race);

/** Set vote status.
    @param [out]  pState   State
    @param [in]   race     Player
    @param [in]   flag     true if player voted to end the game */
void State_SetVote(struct State* pState, RaceType_Def race, Boolean flag);

/** Check whether game is marked finished (transient state).
    @param [in]   pState   State
    @return true to finish */
Boolean State_IsFinished(const struct State* pState);

/** Mark game finished (transient state).
    @param [out]  pState   State
    @param [in]   flag     Flag */
void State_SetFinished(struct State* pState, Boolean flag);

/*
 *  Score
 */

/** Add to a player's score.
    @param [in,out] pState  State
    @param [in]     race    Player
    @param [in]     delta   Score change */
void State_AddScore(struct State* pState, RaceType_Def race, int delta);

/** Get player score.
    @param [in]   pState   State
    @param [in]   race     Player
    @return score */
Int16 State_Score(const struct State* pState, RaceType_Def race);

/** Get player score change (transient state).
    @param [in]   pState   State
    @param [in]   race     Player
    @return score change this turn */
int State_ScoreChange(const struct State* pState, RaceType_Def race);

#endif
