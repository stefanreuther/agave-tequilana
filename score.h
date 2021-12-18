/**
  *  \file score.h
  *  \brief Agave Tequilana - Score Handling
  */
#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

#include "config.h"
#include "state.h"

/** Process build requests.
    Tries to fulfill all requests that have been added using State_SetBuildRequest().

    @param [in,out] pState   State
    @param [in]     pConfig  Configuration */
void ProcessBuildRequests(struct State* pState, const struct Config* pConfig);

/** Compute scores.
    Checks for planets that changed ownership and updates cactuses accordingly.
    Also distributes scores.

    @param [in,out] pState   State
    @param [in]     pConfig  Configuration */
void ComputeScores(struct State* pState, const struct Config* pConfig);

/** Process votes.
    Checks all votes given with State_SetVote() and tries to determine whether the game ends.
    Also publishes the global score reports.

    @param [in,out] pState   State
    @param [in]     pConfig  Configuration */
void ProcessVotes(struct State* pState, const struct Config* pConfig);

/** Send personal reports to players (individual scores and inventories).

    @param [in,out] pState   State
    @param [in]     pConfig  Configuration */
void SendReports(const struct State* pState, const struct Config* pConfig);

#endif
