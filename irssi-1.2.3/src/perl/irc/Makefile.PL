use ExtUtils::MakeMaker;our $AM_DEFAULT_VERBOSITY='0';require "../../../src/perl/Makefile_silent.pm";

WriteMakefile('NAME' => 'Irssi::Irc',
              'LIBS' => '',
	      'OBJECT' => '$(O_FILES)',
              'TYPEMAPS' => ['../common/typemap'],
              'INC' => '-I../../.. -I../../../src -I../../../src/core -I../../../src/irc/core -I../../../src/irc -pthread -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include',
	      'VERSION_FROM' => './Irc.pm');
