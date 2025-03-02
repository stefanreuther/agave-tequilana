# Standard stuff
load_module('Configure.pl');
load_module('Compiler.pl');
find_compiler();
find_archiver();
find_system_libraries(qw(-lm));
find_compiler_options(qw(-g -O -fmessage-length=0 -W -Wall -std=c99));

if ($V{WITH_COVERAGE}) {
    add_to_variable(CXXFLAGS => "-O0 -fprofile-arcs -ftest-coverage");
    add_to_variable(LDFLAGS  => "-fprofile-arcs -ftest-coverage");
}
$V{CFLAGS} = $V{CXXFLAGS}; # d'ooh

# HACK/workaround to get -lpdk and -lm in correct order
my $save = $V{LIBS};
$V{LIBS} = '';

# PDK
find_library('WITH_PDK',
             libs => '-lpdk',
             name => 'PDK',
             program => "#include <phostpdk.h>\nint main() { InitPHOSTLib(); }\n",
             dir => add_variable(PDK_DIR => ''));
die "Unable to find PDK; this is required"
    if !$V{WITH_PDK};
$V{LIBS} = "$V{LIBS} $save";

# Compile stuff
my @SOURCE = qw(
   commands.c
   commands.h
   config.c
   config.h
   language.c
   language.h
   message.c
   message.h
   sendconf.c
   sendconf.h
   score.c
   score.h
   state.c
   state.h
   util.c
   util.h
   utildata.c
   utildata.h
);
compile_static_library('cact', [to_prefix_list($V{IN}, @SOURCE)]);

compile_executable('cactus',
                   [to_prefix_list($V{IN}, qw(main.c))],
                   [qw(cact)]);


# Coverage rules for convenience
if ($V{WITH_COVERAGE}) {
    generate('reset-cov', [], "lcov -q -z -d $V{TMP}");
    rule_set_phony('reset-cov');

    generate('cov', [],
             "lcov -q -c -d $V{TMP} -i -o init.info",
             "lcov -q -c -d $V{TMP} -o capture.info",
             "lcov -q -a init.info -a capture.info -o combined.info",
             "genhtml -t cactus -o report combined.info");
    rule_set_phony('cov');
}
