dnl Libraries.
AX_PKG_CHECK_MODULES([LIBXML], [libxml++-2.6])
AX_BOOST_BASE([1.58.0])
AX_BOOST_SYSTEM
AX_BOOST_FILESYSTEM

# xml depends on utils:
m4_if(cwm4_submodule_dirname, [], [m4_append_uniq([CW_SUBMODULE_SUBDIRS], utils, [ ])])

m4_if(cwm4_submodule_dirname, [], [m4_append_uniq([CW_SUBMODULE_SUBDIRS], cwm4_submodule_basename, [ ])])
m4_append_uniq([CW_SUBMODULE_CONFIG_FILES], cwm4_quote(cwm4_submodule_path[/Makefile]), [ ])
