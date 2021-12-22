/* Language definition: German.
   (The extra parens are to detect accidental extra commas.)
   (Do not use umlauts for charset independence.) */
const struct Language GERMAN = {
    // Message_CommandSyntaxError_Top
    ("(-h0000)<<< Cactus Referee FEHLER >>>\n"
     "\n"
     "Das Kommando:\n"),

    // Message_CommandSyntaxError_Bottom
    ("\nkonnte nicht verstanden werden\n"
     "und wurde ignoriert.\n"),

    // Message_ScoreReport
    ("(-k0000)<<< Cactus Referee >>>\n"
     "\n"
     "Dein Status:\n"
     "------------\n"
     "Kakteen:    %0d [gebaut %1d]\n"
     "Punkte:     %2d\n"
     "Abstimmung: %3B [%4d Stimmen]\n"
     "\n"
     "Agave Tequilana %0V\n"),

    // Message_InventoryReport_Header
    ("(-k0000)<<< Cactus Referee >>>\n"
     "\n"
     "Deine Kakteen:\n"),

    // Message_InventoryReport_Continuation
    ("(-k0000)<<< Cactus Referee >>>\n"
     "\n"
     "Deine Kakteen (Fortsetzung):\n"),

    // ReportScores_Header
    ("(-k0000)<<< Cactus Referee >>>\n"
     "\n"
     "Punktestaende, Zug %0d\n"
     "Volk          Kakteen    Punkte\n"
     "------------  -------  -----------\n"),

    // ReportScores_Footer
    ("\n"
     "Ja-Stimmen: %2d [von %1d, %3d%%]\n"
     "Spiel endet: %4B\n"),

    // Message_CactusCaptured_Previous
    ("(-p%0I)<<< Cactus Referee >>>\n"
     "\n"
     "VON: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Unser KAKTUS wurde erobert von\n"
     "%4R.\n"
     "\n"
     "Punkte: %1d\n"),

    // Message_CactusCaptured_Current
    ("(-p%0I)<<< Cactus Referee >>>\n"
     "\n"
     "VON: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Wir haben einen KAKTUS erobert\n"
     "der vorher\n"
     "%3R\n"
     "gehoerte.\n"
     "\n"
     "Punkte: %2d\n"),

    // Message_CactusLost
    ("(-p%0I)<<< Cactus Referee >>>\n"
     "\n"
     "VON: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Wir haben unseren KAKTUS verloren.\n"
     "\n"
     "Score: %1d\n"),

    // Message_CactusBuilt
    ("(-p%0I)<<< Kaktus errichtet >>>\n"
     "\n"
     "VON: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Ein neuer KAKTUS wurde errichtet.\n"
     "\n"
     "Kosten: %1d\n"),

    // Message_CactusFailed_NotOwned
    ("(-p%0I)<<< Kaktus nicht gebaut >>>\n"
     "\n"
     "VON: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Kann keinen Kaktus errichten:\n"
     "Du kontrollierst diesen Planeten nicht.\n"),

    // Message_CactusFailed_HasFullCactus
    ("(-p%0I)<<< Kaktus nicht gebaut >>>\n"
     "\n"
     "VON: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Kann keinen Kaktus errichten:\n"
     "Der Planet hat bereits einen.\n"),

    // Message_CactusFailed_CannotRebuild
    ("(-p%0I)<<< Kaktus nicht gebaut >>>\n"
     "\n"
     "VON: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Kann keinen Kaktus errichten:\n"
     "Der Planet hat bereits einen Stumpf.\n"),

    // Message_CactusFailed_NeedBase
    ("(-p%0I)<<< Kaktus nicht gebaut >>>\n"
     "\n"
     "VON: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Kann keinen Kaktus errichten:\n"
     "Es wird eine Sternenbasis benoetigt.\n"),

    // Message_CactusFailed_ClansRequired
    ("(-p%0I)<<< Kaktus nicht gebaut >>>\n"
     "\n"
     "VON: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Kann keinen Kaktus errichten:\n"
     "Es werden mindestens %1d Clans\n"
     "benoetigt.\n"),

    // Message_CactusFailed_CactusLimit
    ("(-p%0I)<<< Kaktus nicht gebaut >>>\n"
     "\n"
     "VON: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Kann keinen Kaktus errichten:\n"
     "Du kannst nur %1d Kakteen errichten.\n"),

    // Message_CactusFailed_MinScore
    ("(-p%0I)<<< Kaktus nicht gebaut >>>\n"
     "\n"
     "VON: %0P\n"
     "  Planet %0d\n"
     "\n"
     "Kann keinen Kaktus errichten:\n"
     "Du hast nicht ausreichend Punkte.\n"),

    // Message_VoteIgnored_Turn
    ("(-k0000)<<< Cactus Referee >>>\n\n"
     "Deine Stimme fuer das Spiel-Ende wurde\n"
     "ignoriert, da die Abstimmung noch nicht\n"
     "eroeffnet ist.\n"),

    // Message_VoteIgnored_Build
    ("(-k0000)<<< Cactus Referee >>>\n\n"
     "Deine Stimme fuer das Spiel-Ende wurde\n"
     "ignoriert, da du diesen Zug einen Kaktus\n"
     "errichtet hast.\n"),

    // SendConfig_Header
    ("(-h0000)<<< Cactus Referee >>>\n\n"
     "Agave Tequilana - Konfiguration:\n"),

    // SendConfig_Continuation
    ("(-h0000)<<< Cactus Referee >>>\n\n"
     "Konfiguration (Fortsetzung):\n"),

    // Continuation
    ("(weiter auf der naechsten Seite)\n"),
};
