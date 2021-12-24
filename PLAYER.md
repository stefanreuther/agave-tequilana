Agave Tequilana - Player Documentation
======================================

Agave Tequilana is an add-on for VGA Planets, modeled after the
Tequila War / Cactus add-on.

It implements a scoring system based on keeping and defending planets.



Scenario
--------

The non-profit Cactus Lovers Association iN Galaxy (CLANG) considers
its purpose the popularisation and cultivation of cactuses throughout
the galaxy. (Heretics claim that this religious organisation is just a
cover-up for the mighty, corrupt Galactic Cactus Corporation (GCC),
but that will be none of our business for now.)

CLANG missionaries roam the galaxy, and provide everyone who desires
with seedlings for Agave Tequilana, the, erm, cactus that provides us
with the noble spirit that is Tequila. All they ask in return is a
fair share of the Tequila produced. The canon says that the government
who produces the most Tequila under their reign will be the first to
rule the galaxy.

Now, all the people in the galaxy produce and collect Tequila. It
turns out, however, that invading neighbours' planets and plundering
their Tequila stockpile is a more efficient way to acquire juice.
CLANG on the other hand looks away from the fights... Tequila doesn't
smell, CLANG is happy when they receive their share, no matter from
whom.

In more mundane terms: declare planets important by planting cactuses
to produce score. Defend these planets because losing them gives you a
penalty, conquering them gives a bonus.



Cactus Rules
------------

This add-on keeps a score. The cactuses you own contribute to this
score each turn.


### Building

You can build cactuses on planets you own. To do so, send a message

    cactus: build NNN

to yourself, where NNN is the planet Id. There may be requirements for
the planet (`ClansRequired`, `NeedBase`, `RebuildCactus`), and there
may be an overall limit for the number of cactuses you can build
(`CactusLimit`), but other than that you can build as many as you
want, whenever you want.

Building a cactus may cost score points (`CostAdditive` etc.).
Building cactuses may cause the score to go negative.

As long as you own the cactus, it will produce `TurnScore` points per
turn.

Instead of `build`, you can also write `defhw`.


### Capturing

Capturing a planet with a cactus will award points to the captor
(`CaptureScore`), and give a penalty to the cactus owner
(`LostScore`). Losing a planet entirely gives a different penalty
(`DeadScore`).

**If `KeepCactus=No`,** the cactus will be destroyed and that's it.

**If `KeepCactus=Yes`,** a stump will remain. The stump will continue
producing small amounts of Tequila for the new planet owner and a
penalty for the original builder (`TurnPlusScore`, `TurnMinusScore`,
`TurnOwnerScore`). If enabled (`RebuildCactus`), a new cactus can be
built atop the stump.

Capturing a stump does not give any points.

Building a cactus is therefore worthwhile if you are sure to keep that
planet long enough that it produces more Tequila than you'd pay for
losing it.


### Cactus Status

You will receive a list of your cactuses each turn. Each cactus has
one of the following statuses:

* cactus: it's a full cactus that produces `TurnScore` per turn;

* foreign: it's the stump of a cactus someone else built. You get
  `TurnPlusScore`, the builder gets `TurnMinusScore`;

* stump: it's the stump of a cactus you built, someone captured it,
  and you captured it back. You get `TurnOwnerScore` each turn;

* exile: it's the stump of a cactus you built that someone stole. You
  get `TurnMinusScore`, current owner gets `TurnPlusScore` (this is
  the opposite of "foreign").



Voting Rules
------------

Agave Tequilana can be used to determine the end of a game if enabled
with `EnableFinish` (default).

Starting at `VoteTurn`, players can vote. Each player's vote is worth
their number of planets, plus number of cactuses controlled (i.e.,
planets with a cactus count double). The game ends when more than
`FinishPercent` of all votes want the end.

To vote for the end of the game, send a message

    cactus: vote yes

to yourself. A vote is "sticky" and will be applied for this and
future turns. To retract a vote, send a message

    cactus: vote no

You cannot vote for the end of the game while you are still building
cactuses. Any `build` command (temporarily) invalidates a `vote yes`.

Alternatively, the game will end when someone reaches `FinishScore`.



Friendly Codes
--------------

+ `con` (configuration; planet friendly code)

  Agave Tequilana reacts to the `con` friendly code by sending you the
  current configuration settings as a set of subspace messages.



Host Order
----------

### AUXHOST2 (after movement)

+ Config transmission: process all `con` friendly codes and send
  messages;

+ Command processing: process all commands;

+ Cactus building: cactuses are built in planet Id order, the order in
  which you send the commands does not matter. If a cactus cannot be
  built, it is retried (this can happen rarely, if player A builds a
  cactus over B's stump and therefore frees a slot for B);

+ Score updates;

+ Vote processing;

+ Status reports.



Known Differences to Classic Version
------------------------------------

If you lose a planet the same turn you try to build a cactus, the
build attempt will fail in Agave Tequilana. In Cactus2001, the cactus
will be built and immediately be captured.

In Agave Tequilana, building a cactus always has precedence over
voting "yes". In Cactus2001, it depends on the order in which commands
are received.

Agave Tequilana tries to build cactuses in planet Id order,
independant from the order of commands. Cactus2001 builds cactuses in
the order it receives commands in.



Configuration
-------------

### General

+ `KeepCactus` (boolean, default: `False`)

  When enabled, a stump remains after capturing a planet. When
  disabled, the cactus will be destroyed.


### Scoring

+ `TurnScore` (integer, default: 1)

  Score given to the owner/builder of a cactus every turn.


+ `TurnOwnerScore` (integer, default: 1)

  Score given to the owner of a stump when they were the one who built
  it (i.e. someone captured it and they captured it back). Only
  relevant when `KeepCactus=Yes`.


+ `TurnPlusScore` (integer, default: 1)

  Score given to the owner of a stump they did not build (i.e. they
  captured it). Only relevant when `KeepCactus=Yes`.


+ `TurnMinusScore` (integer, default: -1)

  Score given to the builder of a stump that someone else owns (i.e.
  captured). Only relevant when `KeepCactus=Yes`.


* `CaptureScore` (integer, default: 10)

  Score given to the player who captures a planet with a cactus.


+ `LostScore` (integer, default: -15)

  Score given to the player who loses a planet with a cactus.


+ `DeadScore` (integer, default: -25)

  Score given to the player who loses a planet with a cactus entirely,
  i.e. planet becomes unowned.


### Building

+ `NeedBase` (boolean, default: `False`)

  If enabled, a cactus can only be built on a planet that has a starbase.


+ `RebuildCactus` (boolean, default: `False`)

  When enabled, a cactus can be built on a planet that has a stump.
  When disabled, the stump remains for the remainder of the game.
  Only relevant when `KeepCactus=Yes`.


+ `ClansRequired` (integer, default: 0)

  Minimum number of colonist clans required to build a cactus.


+ `CactusLimit` (integer, default: 0)

  Maximum number of cactuses each player can build. Stumps count
  against that limit even if the player who built them does not
  currently control them.

  The default value 0 (can also be spelled `No`) means the number of
  cactuses is not limited.


+ `CostAdditive` (integer, default: 0), `CostMultiplier` (integer,
  default: 0), `CostPower` (integer, default: 0)

  Determines the cost of a cactus, as

      CostAdditive + CostMultiplier * Num_built_cactuses^CostPower

  Examples:

  - CostAdditive=0, CostMultiplier=0, CostPower=0 (default): building
    cactuses does not affect the score;

  - CostAdditive=7, CostMultiplier=0, CostPower=0: every cactus costs
    7 points;

  - CostAdditive=0, CostMultiplier=1, CostPower=1: arithmetic
    progression, first cactus is free, second costs 1, third costs 2,
    fourth costs 3, etc.

  - CostAdditive=0, CostMultiplier=1, CostPower=2: square progression,
    first cactus is free, second costs 1, third costs 4, fourth costs
    9, etc.

  In the formula, `Num_built_cactuses` is the number of currently
  existing cactuses built by the player, including stumps currently
  under other players' control.


+ `MinScore` (integer, default: -32768)

  Minimum permitted score. If a cactus has a cost greater than 0, it
  is not allowed to reduce the score below this value.

  This option should be configured to a negative value. Because scores
  start out as 0, players need some credit to build the first cactus.

  Losing score from, for example, `CaptureScore`, is not limited and
  can reduce the score below the limit.


### Voting

+ `EnableFinish` (boolean, default: `True`)

  If enabled, Agave Tequilana can be used to determine the end of a
  game, either by voting, or by reaching a score goal. The following
  parameters are only handled if this option is enabled (default).

  When disabled, `vote` commands are silently ignored and the
  following parameters have no effect.


+ `VoteTurn` (integer, default: 65)

  Votes are accepted for the first time at this turn.


+ `FinishPercent` (integer, default: 66)

  Game finishes if this percentage of the given votes approve the end.


+ `FinishScore` (integer, default: 2000)

  Game finishes when one player reaches this many points.



Utility Data Files
------------------

Agave Tequilana will generate `utilX.dat` records.


### Player score (type 51)

This record is defined by PHost to be used for score reports. Agave
Tequilana sends three copies of it.

    50 BYTEs   Name; player-readable description of this score.
       WORD    Identifier
                 16544  Number of owned cactuses
                 16545  Number of built cactuses
                 16546  Score
       WORD    Turns-over-limit required to win, sent as -1.
       DWORD   Win limit. For the score, same as FinishScore,
               otherwise, -1 for none.
    11 DWORDs  Scores for each player. -1 if not known. Negative scores
               are reported as 0.


### Your score (type 16544)

This record is sent every turn and reports your score. It also serves
as a marker that this add-on is present.

    32 BYTEs   Name and version of add-on, "Agave Tequilana 0.42"
       WORD    Number of owned cactuses
       WORD    Number of built cactuses
       WORD    Score (can be negative)
       WORD    Vote flag


### Cactus (type 16545)

This record is sent every turn, for every cactus you built or own.

    WORD    Planet Id
    WORD    Type
             0      Cactus
             1      Foreign stump (someone else built, you own)
             2      Exile stump (you built, someone else owns)
             3      Stump (you built and own)
