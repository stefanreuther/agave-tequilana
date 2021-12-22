/**
  *  \file score.c
  *  \brief Agave Tequilana - Score Handling
  *
  *  This module contains the main logic of the add-on.
  */

#include <phostpdk.h>
#include <stdlib.h>
#include "score.h"
#include "message.h"
#include "language.h"


/*
 *  Buiding
 */

enum Result {
    Success,
    Fail_NotOwned,
    Fail_HasFullCactus,
    Fail_CannotRebuild,
    Fail_NeedBase,
    Fail_ClansRequired,
    Fail_CactusLimit,
    Fail_MinScore
};

/* Compute a * b^exp. */
static int Power(int a, int b, int exp)
{
    // Positive exponent
    for (int i = 0; i < exp; ++i) {
        a *= b;
    }

    // Negative exponent
    for (int i = 0; i > exp; --i) {
        a /= b;
    }
    return a;
}

/* Compute cost for one cactus. */
static int CactusCost(const struct State* pState, const struct Config* pConfig, RaceType_Def owner, Boolean buildingOverStump)
{
    return pConfig->CostAdditive + Power(pConfig->CostMultiplier, State_NumBuiltCactuses(pState, owner) - (int)buildingOverStump, pConfig->CostPower);
}

/* Process a single build request.
   Will either build the cactus and send necessary messages,
   or not build the cactus and return a failure status. */
static enum Result ProcessBuildRequest(struct State* pState, const struct Config* pConfig, const Uns16 planetId)
{
    // Command has been validated against State_PlanetOwner.
    // Check whether that still is current or player has lost the planet.
    const RaceType_Def race = PlanetOwner(planetId);
    if (race != State_PlanetOwner(pState, planetId)) {
        return Fail_NotOwned;
    }

    // Cannot build if there is already a full cactus.
    if (State_PlanetHasFullCactus(pState, planetId)) {
        return Fail_HasFullCactus;
    }

    // Cannot build over a stump unless allowed.
    if (State_PlanetHasCactus(pState, planetId) && !pConfig->RebuildCactus) {
        return Fail_CannotRebuild;
    }

    // Might require a base.
    if (pConfig->NeedBase && !IsBaseExist(planetId)) {
        return Fail_NeedBase;
    }

    // Might require clans.
    if (PlanetCargo(planetId, COLONISTS)/100 < (Uns32)pConfig->ClansRequired) {
        return Fail_ClansRequired;
    }

    // Check limit.
    // Note that building over an own stump must be treated specially because it doesn't change the net count.
    const Boolean buildingOverStump = (State_PlanetHasCactus(pState, planetId) && State_CactusBuilder(pState, planetId) == race);
    if (pConfig->CactusLimit > 0 && State_NumBuiltCactuses(pState, race) - (Int16)buildingOverStump >= pConfig->CactusLimit) {
        return Fail_CactusLimit;
    }

    // Compute cost. Must be done before State_CreateCactus() to use correct count.
    const int cost = CactusCost(pState, pConfig, race, buildingOverStump);

    // Might exceed limit
    // Special case for MinScore <= -32768, so if option is not set, it has no effect.
    Int16 currentScore = State_Score(pState, race);
    if (pConfig->MinScore > -32768
        && (currentScore < pConfig->MinScore
            || cost > (Int32)currentScore - pConfig->MinScore))
    {
        return Fail_MinScore;
    }

    // All conditions pass, do it
    Info("\t(+) build cactus: planet %d, player %d, cost %d", planetId, race, cost);
    State_CreateCactus(pState, planetId, race);
    State_AddScore(pState, race, -cost);

    // Messaging
    Message_CactusBuilt(race, planetId, cost);

    return Success;
}

void ProcessBuildRequests(struct State* pState, const struct Config* pConfig)
{
    // Perform building in a loop.
    // Building cactus A may enable cactus B being built when A builds over a stump built by B
    // and therefore reduces B's NumBuiltCactuses below CactusLimit.
    while (1) {
        Info("    Building...");

        Boolean did = False;
        for (Uns16 planetId = 1; planetId <= PLANET_NR; ++planetId) {
            if (State_HasBuildRequest(pState, planetId)) {
                if (ProcessBuildRequest(pState, pConfig, planetId) == Success) {
                    State_SetBuildRequest(pState, planetId, False);
                    did = True;
                }
            }
        }

        if (!did) {
            break;
        }
    }

    // Everything that remains is an error.
    for (Uns16 planetId = 1; planetId <= PLANET_NR; ++planetId) {
        if (State_HasBuildRequest(pState, planetId)) {
            RaceType_Def owner = State_PlanetOwner(pState, planetId);
            switch (ProcessBuildRequest(pState, pConfig, planetId)) {
             case Success:
                // Cannot happen
                break;
             case Fail_NotOwned:
                Message_CactusFailed_NotOwned(owner, planetId);
                break;
             case Fail_HasFullCactus:
                Message_CactusFailed_HasFullCactus(owner, planetId);
                break;
             case Fail_CannotRebuild:
                Message_CactusFailed_CannotRebuild(owner, planetId);
                break;
             case Fail_NeedBase:
                Message_CactusFailed_NeedBase(owner, planetId);
                break;
             case Fail_ClansRequired:
                Message_CactusFailed_ClansRequired(owner, planetId, pConfig->ClansRequired);
                break;
             case Fail_CactusLimit:
                Message_CactusFailed_CactusLimit(owner, planetId, pConfig->CactusLimit);
                break;
             case Fail_MinScore:
                Message_CactusFailed_MinScore(owner, planetId);
                break;
            }
        }
    }
}

/*
 *  Score Computation
 */

/* Compute score for a single planet */
static void ComputeScore(Uns16 planetId, struct State* pState, const struct Config* pConfig)
{
    const RaceType_Def currentOwner = PlanetOwner(planetId);
    const RaceType_Def previousOwner = State_PlanetOwner(pState, planetId);

    // Check for ownership change
    if (State_PlanetHasCactus(pState, planetId) && currentOwner != previousOwner) {
        if (State_PlanetHasFullCactus(pState, planetId)) {
            if (currentOwner != NoRace) {
                // Capturing a cactus
                Info("\tCactus %d, owned by %d, captured by %d", planetId, previousOwner, currentOwner);
                State_AddScore(pState, previousOwner, pConfig->LostScore);
                State_AddScore(pState, currentOwner,  pConfig->CaptureScore);
                Message_CactusCaptured(previousOwner, currentOwner, planetId, pConfig->LostScore, pConfig->CaptureScore);
            } else {
                // Destroyed
                Info("\tCactus %d, owned by %d, lost", planetId, previousOwner);
                State_AddScore(pState, previousOwner, pConfig->DeadScore);
                Message_CactusLost(previousOwner, planetId, pConfig->DeadScore);
            }
        }

        // Destroy or damage cactus
        State_RemoveCactus(pState, planetId, pConfig->KeepCactus);
    }

    // Give per-turn points
    if (State_PlanetHasCactus(pState, planetId)) {
        if (State_PlanetHasFullCactus(pState, planetId)) {
            // Full cactus
            State_AddScore(pState, currentOwner, pConfig->TurnScore);
        } else {
            // Stump
            const RaceType_Def builder = State_CactusBuilder(pState, planetId);
            if (currentOwner == builder) {
                State_AddScore(pState, currentOwner, pConfig->TurnOwnerScore);
            } else {
                State_AddScore(pState, currentOwner, pConfig->TurnPlusScore);
                State_AddScore(pState, builder,      pConfig->TurnMinusScore);
            }
        }
    }

    State_SetPlanetOwner(pState, planetId, currentOwner);
}

void ComputeScores(struct State* pState, const struct Config* pConfig)
{
    Info("    Updating scores...");
    for (Uns16 planetId = 1; planetId <= PLANET_NR; ++planetId) {
        ComputeScore(planetId, pState, pConfig);
    }
}


/*
 *  Voting
 */

struct VoteItem {
    RaceType_Def Player;
    int Votes;
    int Score;
};

/* Compare two VoteItems. */
static int CompareVotes(const void* a, const void* b)
{
    const struct VoteItem* va = a;
    const struct VoteItem* vb = b;

    if (va->Score != vb->Score) {
        return va->Score < vb->Score ? 1 : -1;
    } else if (va->Votes != vb->Votes) {
        return va->Votes < vb->Votes ? 1 : -1;
    } else {
        return va->Player < vb->Player ? -1 : 1;
    }
}

/* Formula: votes per player */
static int GetPlayerVotes(const struct State* pState, RaceType_Def player)
{
    return State_CountPlanets(pState, player)
        + State_NumOwnedCactuses(pState, player);
}

/* Report scores for a single player. */
static void ReportScores(RaceType_Def to,
                         const struct State* pState,
                         const struct VoteItem* votes,
                         size_t numPlayers,
                         Int32 totalVotes,
                         Int32 yesVotes)
{
    const struct Language*const lang = GetLanguageForPlayer(to);

    const Int32 args[] = {
        TurnNumber(),                                  // 0
        totalVotes,                                    // 1
        yesVotes,                                      // 2
        totalVotes > 0 ? yesVotes*100/totalVotes : 0,  // 3
        State_IsFinished(pState)                       // 4
    };

    struct Message m;
    Message_Init(&m);
    Message_Format(&m, lang->ReportScores_Header, args, 5);
    for (size_t i = 0; i < numPlayers; ++i) {
        const RaceType_Def r = votes[i].Player;
        char tmp[100];
        sprintf(tmp, "%-12s  %3d %+3d  %5d %+5d\n",
                RaceNameAdjective(r, 0),
                State_NumOwnedCactuses(pState, r),
                State_NumOwnedCactusesChange(pState, r),
                State_Score(pState, r),
                State_ScoreChange(pState, r));
        Message_Add(&m, tmp);
    }
    Message_Format(&m, lang->ReportScores_Footer, args, 5);
    Message_Send(&m, to);
}

static void SaveRefereeFile(FILE* fp, const struct VoteItem* votes, size_t numPlayers, Boolean isFinished)
{
    for (size_t i = 0; i < numPlayers; ++i) {
        fprintf(fp, "rank%d=%d\n", (int)votes[i].Player, (int) (i+1));
    }
    fprintf(fp, "end=%d\n", isFinished ? 1 : 0);
}

void ProcessVotes(struct State* pState, const struct Config* pConfig, Boolean writeRef)
{
    struct VoteItem votes[RACE_NR];
    size_t numPlayers = 0;
    Int32 totalVotes = 0;
    Int32 yesVotes = 0;
    Info("    Processing votes...");

    // Gather players and collect votes
    for (int i = 1; i <= RACE_NR; ++i) {
        RaceType_Def r = (RaceType_Def) i;
        if (PlayerIsActive(r)) {
            // Remember for sorting
            int ourVotes = GetPlayerVotes(pState, r);
            votes[numPlayers].Player = r;
            votes[numPlayers].Votes = ourVotes;
            votes[numPlayers].Score = State_Score(pState, r);
            ++numPlayers;

            // Count votes
            totalVotes += ourVotes;
            if (State_HasVote(pState, r)) {
                if (TurnNumber() < pConfig->VoteTurn) {
                    // Ignore
                    Message_VoteIgnored_Turn(r);
                    Info("\t(-) player %d vote ignored: turn not reached", i);
                } else if (State_NumCactusesBuiltThisTurn(pState, r) != 0) {
                    // Cancel vote
                    Message_VoteIgnored_Build(r);
                    Info("\t(-) player %d vote ignored: built a cactus this turn", i);
                } else {
                    // Count vote
                    Info("\t(-) player %d votes to end with %d votes", i, (int) ourVotes);
                    yesVotes += ourVotes;
                }
            }
        }
    }

    // Degenerate case
    if (numPlayers == 0) {
        return;
    }

    // Determine top scores
    qsort(votes, numPlayers, sizeof(votes[0]), CompareVotes);

    // Determine game end
    const Boolean isFinished =
        (votes[0].Score >= pConfig->FinishScore)
        || (totalVotes > 0
            && (100*yesVotes) >= (totalVotes*pConfig->FinishPercent));
    State_SetFinished(pState, isFinished);
    Info("\tbest score: %d, votes: %d/%d -> %s", (int) votes[0].Score, (int) yesVotes, (int) totalVotes,
         isFinished ? "game FINISHED" : "game proceeds");

    // Inform players
    for (int i = 1; i <= RACE_NR; ++i) {
        const RaceType_Def r = (RaceType_Def) i;
        if (PlayerIsActive(r)) {
            ReportScores(r, pState, votes, numPlayers, totalVotes, yesVotes);
        }
    }

    // Referee file for c2host
    if (writeRef) {
        FILE* fp = OpenOutputFile("c2ref.txt", GAME_DIR_ONLY);
        SaveRefereeFile(fp, votes, numPlayers, isFinished);
        fclose(fp);
    }
}


/*
 *  Reporting
 */

/* Send score report to single player. */
static void SendScoreReport(const struct State* pState, RaceType_Def player)
{
    const int numOwnedCactuses = State_NumOwnedCactuses(pState, player);
    const int numBuiltCactuses = State_NumBuiltCactuses(pState, player);
    const Int16 score          = State_Score(pState, player);
    const Boolean hasVote      = State_HasVote(pState, player);
    const int numVotes         = GetPlayerVotes(pState, player);

    Message_ScoreReport(player, numOwnedCactuses, numBuiltCactuses, score, hasVote, numVotes);
}

/* Send inventory report to single player.
   This report can span multiple messages. */
static void SendInventoryReport(const struct State* pState, RaceType_Def player)
{
    const struct Language*const lang = GetLanguageForPlayer(player);
    struct Message m;
    Message_Init(&m);
    Message_Add(&m, lang->Message_InventoryReport_Header);
    Boolean hasText = False;
    for (Uns16 planetId = 1; planetId <= PLANET_NR; ++planetId) {
        if (State_PlanetHasCactus(pState, planetId)
            && (PlanetOwner(planetId) == player || State_CactusBuilder(pState, planetId) == player))
        {
            const char* what = State_PlanetHasFullCactus(pState, planetId)
                ? "cactus"
                : State_CactusBuilder(pState, planetId) != player
                ? "foreign"
                : PlanetOwner(planetId) == player
                ? "stump"
                : "exile";
            char tmp[100];
            sprintf(tmp, "%4d  %-20s  %s\n", planetId, PlanetName(planetId, 0), what);
            Message_Add(&m, tmp);

            if (m.Lines >= MAX_MESSAGE_LINES) {
                Message_Add(&m, lang->Continuation);
                Message_Send(&m, player);
                Message_Init(&m);
                Message_Add(&m, lang->Message_InventoryReport_Continuation);
                hasText = False;
            } else {
                hasText = True;
            }
        }
    }

    if (hasText) {
        Message_Send(&m, player);
    }
}

void SendReports(const struct State* pState, const struct Config* pConfig)
{
    (void) pConfig;

    Info("    Sending reports...");
    for (int i = 1; i <= RACE_NR; ++i) {
        if (PlayerIsActive(i)) {
            SendScoreReport(pState, i);
            SendInventoryReport(pState, i);
        }
    }
}

void SaveScoreFile(const struct State* pState)
{
    FILE* fp = OpenOutputFile("c2score.txt", GAME_DIR_ONLY);
    fprintf(fp, "%% score\n");
    fprintf(fp, "description=Tequila\n");
    for (int i = 1; i <= RACE_NR; ++i) {
        if (PlayerIsActive(i)) {
            fprintf(fp, "score%d=%d\n", i, (int) State_Score(pState, (RaceType_Def) i));
        }
    }
    fclose(fp);
}
