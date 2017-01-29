# ai-xml submodule

This repository is a [git submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules)
providing a C++ framework for serializing classes to and from XML with
a minimal amount of code per class.

The root project should be using
[autotools](https://en.wikipedia.org/wiki/GNU_Build_System autotools),
[cwm4](https://github.com/CarloWood/cwm4) and
[libcwd](https://github.com/CarloWood/libcwd).

## Example

For example, a program that can read and write the
xml file [catalog.xml](https://github.com/CarloWood/ai-xml/blob/master/catalog_test.xml)
could look something like [this](https://github.com/CarloWood/ai-xml/blob/master/catalog_test.cxx).

## Checking out a project that uses the ai-xml submodule.

To clone a project example-project that uses ai-xml simply run:

<pre>
<b>git clone --recursive</b> &lt;<i>URL-to-project</i>&gt;<b>/example-project.git</b>
<b>cd example-project</b>
<b>./autogen.sh</b>
</pre>

The <tt>--recursive</tt> is optional because <tt>./autogen.sh</tt> will fix
it when you forgot it.

Afterwards you probably want to use <tt>--enable-mainainer-mode</tt>
as option to the generated <tt>configure</tt> script.

## Adding the ai-xml submodule to a project

To add this submodule to a project, that project should already
be set up to use [cwm4](https://github.com/CarloWood/cwm4).

Simply execute the following in a directory of that project
where you what to have the <tt>xml</tt> subdirectory:

<pre>
git submodule add https://github.com/CarloWood/ai-xml.git xml
</pre>

This should clone ai-xml into the subdirectory <tt>xml</tt>, or
if you already cloned it there, it should add it.

Changes to <tt>configure.ac</tt> and <tt>Makefile.am</tt>
are taken care of my <tt>cwm4</tt>, except for linking:

For example a module that defines a

<pre>
bin_PROGRAMS = foobar
</pre>

would also define

<pre>
foobar_CXXFLAGS = @LIBXML_CFLAGS@ @LIBCWD_FLAGS@
foobar_LDADD = ../xml/libxml.la ../utils/libutils.la ../cwd/libcwd.la @BOOST_FILESYSTEM_LIB@ @BOOST_SYSTEM_LIB@ @LIBXML_LIBS@ @LIBCWD_LIBS@
</pre>

or whatever the path to `xml` etc. is, to link with libxml, and
assuming you'd also use the [cwd](https://github.com/CarloWood/cwd) submodule.

Finally, run

<pre>
./autogen.sh
</pre>

to let cwm4 do its magic, and commit all the changes.

Checkout [ai-xml-testsuite](https://github.com/CarloWood/ai-xml-testsuite)
for an example of a project that uses this submodule.
