/**
  *  \file commands.h
  *  \brief Agave Tequilana - Command Handling
  */
#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

#include "config.h"
#include "state.h"

/** Process commands.
    Reads all commands given by players and updates them in pState.

    Commands are verified against pState (=previous turn's state),
    not the current universe state.

    @param [in,out] pState   Game state
    @param [in]     pConfig  Configuration */
void ProcessCommands(struct State* pState, const struct Config* pConfig);

#endif
