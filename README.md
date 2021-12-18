Agave Tequilana
===============

Agave Tequilana is an add-on for VGA Planets, modeled after the
Tequila War / Cactus add-on. See PLAYER.md for player documentation.

It implements a scoring system based on keeping and defending planets.


Compiling
---------

You need the PHost development kit (PDK) that is available here:
<http://phost-contrib.sourceforge.net/>

**If you just want it compiled,** and that's it, use the enclosed
Makefile. Adjust the variables at the top and type `make`.

**If you want to seriously modify it,** use the Makefile generator
from <https://github.com/stefanreuther/accidental-build>. Create a
build directory, and type

     /path/to/Make.pl IN=/path/to/source PDK_DIR=/path/to/pdk
     make

This will create a Makefile suited for development, which does
automatic dependencies, automatic rebuild on rule change, and
out-of-tree build.

In any case, the build result will be a binary `cactus` that is the
entire add-on.

So far, Agave Tequilana has been tested only on Linux with PHost.


Installing and Configuring
--------------------------

Agave Tequilana needs to be invoked from AUXHOST2.INI. Invoke as

    cactus path/to/game

from the respective location.

Agave Tequilana will take a configuration file `cactus.src` from
the game directory. You can use `cactus -dc` on an empty directory
to print a list of configuration options with defaults. See PLAYER.md
for descriptions of the options.

Agave Tequilana will store state in a file `cactus.hst` in the game
directory. This file should be compatible with Tequila War / Cactus.


Colophon
--------

This add-on re-implements the Tequila War / Cactus scoring system
(cact2001.zip) by Eugene Goroh and Konst Kopytov, translated by Gleb
Mazursky. It uses the same file formats.

Written in 2021 by Stefan Reuther <streu@gmx.de> for PlanetsCentral
<https://planetscentral.com/>.

This source code is licensed under a permissive BSD license. Note that
linking against the PDK will produce a binary that is covered by the
GNU GPL, as per the PDK's license.
