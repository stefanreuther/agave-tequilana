/**
  *  \file sendconf.h
  *  \brief Agave Tequilana - Send Configuration to Players
  */
#ifndef SENDCONF_H_INCLUDED
#define SENDCONF_H_INCLUDED

struct Config;

/** Send configuration to players who requested it ("con" fcode).
    @param [in] c Configuration */
void DoSendConfig(const struct Config* c);

#endif
