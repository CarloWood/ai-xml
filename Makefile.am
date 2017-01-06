AM_CPPFLAGS = -iquote $(top_srcdir) -iquote $(top_srcdir)/cwd

noinst_LTLIBRARIES = libxml.la
bin_PROGRAMS = catalog_test example_test

libxml_la_SOURCES = \
	Bridge.cpp \
	Bridge.h \
	escape.cpp \
	escape.h \
	Writer.cpp \
	Writer.h \
	ReadBridge.cpp \
	ReadBridge.h \
	WriteBridge.cpp \
	WriteBridge.h \
	Reader.cpp \
	Reader.h \
	read_from_string.cpp \
	read_from_string.h \
	read_from_stream.cpp \
	read_from_stream.h \
	write_to_stream.cpp \
	write_to_stream.h \
	SetLocale.cpp \
	SetLocale.h \
	set_locale_for.h \
	write_to_string.h

catalog_test_SOURCES = \
	catalog_test.cpp

example_test_SOURCES = \
	example_test.cpp

libxml_la_CXXFLAGS = @LIBXML_CFLAGS@ @LIBCWD_FLAGS@

catalog_test_CXXFLAGS = @LIBXML_CFLAGS@ @LIBCWD_FLAGS@
catalog_test_LDADD = libxml.la ../utils/libutils.la ../cwd/libcwd.la @BOOST_FILESYSTEM_LIB@ @BOOST_SYSTEM_LIB@ @LIBXML_LIBS@ @LIBCWD_LIBS@

example_test_CXXFLAGS = @LIBXML_CFLAGS@ @LIBCWD_FLAGS@
example_test_LDADD = libxml.la ../utils/libutils.la ../cwd/libcwd.la @BOOST_FILESYSTEM_LIB@ @BOOST_SYSTEM_LIB@ @LIBXML_LIBS@ @LIBCWD_LIBS@

# --------------- Maintainer's Section

if MAINTAINER_MODE
distclean-local:
	rm -f *.s *.ii

clean-local:
endif

MAINTAINERCLEANFILES = $(srcdir)/Makefile.in
