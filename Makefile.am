EXTRA_DIST = *.h

noinst_LTLIBRARIES = libxml.la
bin_PROGRAMS = catalog_test example_test

# See src/Makefile.am
DEFS = @DEFS@
DEFAULT_INCLUDES=-iquote $(top_builddir) -iquote $(top_builddir)/src -iquote $(srcdir) -iquote $(srcdir)/.. -iquote $(srcdir)/../cwd

# See src/Makefile.am.
libxml_la_CXXFLAGS = @LIBXML_CFLAGS@ @CXXFLAGS@ @CWD_FLAGS@
libxml_la_SOURCES = \
	Bridge.cpp \
	escape.cpp \
	Writer.cpp \
	ReadBridge.cpp \
	WriteBridge.cpp \
	Reader.cpp \
	read_from_string.cpp \
	read_from_stream.cpp \
	write_to_stream.cpp \
	SetLocale.cpp

catalog_test_SOURCES = \
	catalog_test.cpp

example_test_SOURCES = \
	example_test.cpp

catalog_test_CXXFLAGS = @LIBXML_CFLAGS@ @CXXFLAGS@ @CWD_FLAGS@
catalog_test_LDFLAGS =
catalog_test_LDADD = .libs/libxml.a ../utils/.libs/libutils.a ../cwd/.libs/libdebug.a @LIBXML_LIBS@ @LIBS@ @CWD_LIBS@
catalog_test_DEPENDENCIES = libxml.la ../utils/libutils.la ../cwd/libdebug.la

example_test_CXXFLAGS = @LIBXML_CFLAGS@ @CXXFLAGS@ @CWD_FLAGS@
example_test_LDFLAGS =
example_test_LDADD = .libs/libxml.a ../utils/.libs/libutils.a ../cwd/.libs/libdebug.a @LIBXML_LIBS@ @LIBS@ @CWD_LIBS@
example_test_DEPENDENCIES = libxml.la ../utils/libutils.la ../cwd/libdebug.la

MAINTAINERCLEANFILES = $(srcdir)/Makefile.in

if MAINTAINER_MODE
distclean-local:
	rm -f *.s *.ii

clean-local:
endif
