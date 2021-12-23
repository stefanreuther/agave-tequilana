/* Language definition: English.
   (The extra parens are to detect accidental extra commas.) */
const struct Language ENGLISH = {
    // Message_CommandSyntaxError_Top
    ("(-h0000)<<< Cactus Referee ERROR >>>\n"
     "\n"
     "The command:\n"),

    // Message_CommandSyntaxError_Bottom
    ("\nwas not understood and has been ignored.\n"),

    // Message_ScoreReport
    ("(-k0000)<<< Cactus Referee >>>\n"
     "\n"
     "Your status:\n"
     "------------\n"
     "Cactuses: %0d [built %1d]\n"
     "Score:    %2d\n"
     "Vote:     %3B [%4d votes]\n"
     "\n"
     "Agave Tequilana %0V\n"),

    // Message_InventoryReport_Header
    ("(-k0000)<<< Cactus Referee >>>\n"
     "\n"
     "Your cactuses:\n"),

    // Message_InventoryReport_Continuation
    ("(-k0000)<<< Cactus Referee >>>\n"
     "\n"
     "Your cactuses (continued):\n"),

    // ReportScores_Header
    ("(-k0000)<<< Cactus Referee >>>\n"
     "\n"
     "Scores, turn %0d\n"
     "Race         Cactuses     Score\n"
     "------------  -------  -----------\n"),

    // ReportScores_Footer
    ("\n"
     "Yes Votes: %2d [of %1d, %3d%%]\n"
     "Game ends: %4B\n"),

    // Message_CactusCaptured_Previous
    ("(-p%0I)<<< Cactus Referee >>>\n"
     "\n"
     "FROM: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Our CACTUS was captured by\n"
     "%4R.\n"
     "\n"
     "Score: %1d\n"),

    // Message_CactusCaptured_Current
    ("(-p%0I)<<< Cactus Referee >>>\n"
     "\n"
     "FROM: %0P\n"
     "  Planet %0d\n"
     "\n"
     "We have captured a CACTUS\n"
     "previously owned by\n"
     "%3R.\n"
     "\n"
     "Score: %2d\n"),

    // Message_CactusLost
    ("(-p%0I)<<< Cactus Referee >>>\n"
     "\n"
     "FROM: %0P\n"
     "  Planet %0d\n"
     "\n"
     "We have lost our CACTUS.\n"
     "\n"
     "Score: %1d\n"),

    // Message_CactusBuilt
    ("(-p%0I)<<< Cactus Built >>>\n"
     "\n"
     "FROM: %0P\n"
     "  Planet %0d\n"
     "\n"
     "New CACTUS has been built.\n"
     "\n"
     "Cost: %1d\n"),

    // Message_CactusFailed_NotOwned
    ("(-p%0I)<<< Cactus Not Built >>>\n"
     "\n"
     "FROM: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Cannot build a new cactus:\n"
     "You do not control that planet.\n"),

    // Message_CactusFailed_HasFullCactus
    ("(-p%0I)<<< Cactus Not Built >>>\n"
     "\n"
     "FROM: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Cannot build a new cactus:\n"
     "Planet already has one.\n"),

    // Message_CactusFailed_CannotRebuild
    ("(-p%0I)<<< Cactus Not Built >>>\n"
     "\n"
     "FROM: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Cannot build a new cactus:\n"
     "Planet already has a stump.\n"),

    // Message_CactusFailed_NeedBase
    ("(-p%0I)<<< Cactus Not Built >>>\n"
     "\n"
     "FROM: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Cannot build a new cactus:\n"
     "You need a starbase to build a cactus.\n"),

    // Message_CactusFailed_ClansRequired
    ("(-p%0I)<<< Cactus Not Built >>>\n"
     "\n"
     "FROM: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Cannot build a new cactus:\n"
     "You need at least %1d clans.\n"),

    // Message_CactusFailed_CactusLimit
    ("(-p%0I)<<< Cactus Not Built >>>\n"
     "\n"
     "FROM: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Cannot build a new cactus:\n"
     "You can only build %1d cactuses.\n"),

    // Message_CactusFailed_MinScore
    ("(-p%0I)<<< Cactus Not Built >>>\n"
     "\n"
     "FROM: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Cannot build a new cactus:\n"
     "Your score is too low.\n"),

    // Message_VoteIgnored_Turn
    ("(-k0000)<<< Cactus Referee >>>\n\n"
     "Your vote to end the game has been\n"
     "ignored because voting is not yet open.\n"),

    // Message_VoteIgnored_Build
    ("(-k0000)<<< Cactus Referee >>>\n\n"
     "Your vote to end the game has been\n"
     "ignored because you built a cactus\n"
     "this turn.\n"),

    // SendConfig_Header
    ("(-h0000)<<< Cactus Referee >>>\n\n"
     "Agave Tequilana - Configuration:\n"),

    // SendConfig_Continuation
    ("(-h0000)<<< Cactus Referee >>>\n\n"
     "Configuration (continued):\n"),

    // Continuation
    ("(continued on next page)\n"),

    // Score_NumOwnedCactuses
    ("Owned Cactuses"),

    // Score_NumBuiltCactuses
    ("Built Cactuses"),

    // Score_Score
    ("Tequila Score"),
};
