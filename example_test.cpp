/**
 * \file example_test.cpp
 * \brief Example code that shows how to use ai-xml.
 *
 * Copyright (C) 2014 Aleric Inglewood.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef USE_PCH
#include "sys.h"
#endif

#include "utils/AIAlert.h"
#include "xml/Reader.h"
#include "xml/Writer.h"

#ifndef USE_PCH
#include "debug.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <set>
#endif

#undef PRINT_DEBUG

namespace fs = boost::filesystem;

class Version2				// <version2
{
  protected:
    // Attributes.
    int m_value;			//           value="2" />

  public:
    void xml(xml::Bridge& xml);
};

void Version2::xml(xml::Bridge& xml)
{
  xml.node_name("version2");
  xml.attribute("value", m_value);
}

class Version5 : public Version2	// <version5              value="5" /> (or just <version5 />)
{
  private:
    // Attributes.
    float m_empty;			//            empty="1.0"

  public:
    void xml(xml::Bridge& xml);

    float empty() const { return m_empty; }
};

void Version5::xml(xml::Bridge& xml)
{
  xml.node_name("version5");
  xml.optional_attribute("value", m_value);
  xml.optional_attribute("empty", m_empty);
}

class Foobar				// <foobar
{
  private:
    // Attributes.
    float m_version;			//         version="5"
    std::string m_optional;		//         optional="this element sees version=2"/"this one does"
    // Children.
    Version2 m_version2;		//   <version2 ...>  (only in the first <foobar>)
    Version5 m_version5;		//   <version5 ...>  (only in the second and last <foobar>)
    std::set<float> m_params;		//   <param />           (only in the last <foobar>)
					//   <param>4.3</param>  (only in the last <foobar>)
					//   <param>5.6</param>  (only in the last <foobar>)
    std::vector<std::string> m_strings;	//   <str>...</str>  (x3, only in the last <foobar>)
					// </foobar>

  public:
    void xml(xml::Bridge& xml);
};

void Foobar::xml(xml::Bridge& xml)
{
  xml.node_name("foobar");
  xml.optional_attribute("version", m_version, 1.0f);
  if (m_version != 1.0f)
  {
    xml.set_version(m_version);
  }
  xml.optional_attribute("optional", m_optional, std::string("foobar!"));
  if (xml.version() == 2)
  {
    xml.child(m_version2);
  }
  else
  {
    xml.child(m_version5);
    xml.children_stream("param", m_params, xml::insert);
    if (!xml.writing())
    {
      Dout(dc::notice, "Read " << m_params.size() << " <param> elements:");
      for (auto iter = m_params.begin(); iter != m_params.end(); ++iter)
	Dout(dc::notice, "  " << *iter);
    }
    xml.children_stream("str", m_strings);
    if (!xml.writing())
    {
      Dout(dc::notice, "Read " << m_strings.size() << " <str> elements:");
      for (auto iter = m_strings.begin(); iter != m_strings.end(); ++iter)
	Dout(dc::notice, "  \"" << *iter << "\"");
    }
  }
}

class Example				// <example
{
  private:
    // Attributes.
    float m_version;			//          version="2.3"
    float m_mandatory;			//                        mandatory="3.1425"
    std::string m_optional;		//                                           optional="hello world">
    // Children.
    std::vector<Foobar> m_foobar_vec;	//   <foobar...
					//   </foobar>
					//   <foobar ...
					//   </foobar>
					//   <foobar ...>
					//   </foobar>
					// </example>

  public:
    void xml(xml::Bridge& xml);
};

void Example::xml(xml::Bridge& xml)
{
  xml.node_name("example");
  xml.attribute("version", m_version);
  xml.set_version(m_version);
  xml.attribute("mandatory", m_mandatory);
  xml.optional_attribute("optional", m_optional, std::string("MISSING"));
  xml.children(m_foobar_vec);

  if (!xml.writing())
  {
    Dout(dc::notice, "Read " << m_foobar_vec.size() << " <foobar> elements.");
  }
}

int main(int argc, char* argv[])
{
  Debug(debug::init());

  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " example_test.xml" << std::endl;
    return 1;
  }

  fs::path filepath(argv[1]);
  xml::Reader reader;

  Example example;
  try
  {
    reader.parse(filepath, 1);
    example.xml(reader);
  }
  catch(AIAlert::Error const& error)
  {
    std::cerr << AIAlert::Error(AIAlert::Prefix(), AIAlert::not_modal, "Failed to load [FILE]: ",
	AIArgs("[FILE]", filepath.native()), error) << std::endl;
    return 1;
  }
  catch(std::runtime_error const& error)
  {
    std::cerr << "Failed to load " << filepath << ": " << error.what() << std::endl;
    return 1;
  }

#ifdef PRINT_DEBUG
  DebugBuf buf;
  std::ostream dout(&buf);
  xml::Writer writer(dout);
#else
  Debug(libcw_do.off());
  xml::Writer writer(std::cout);
#endif

  writer.write(example);

#ifndef PRINT_DEBUG
  Debug(libcw_do.on());
#endif
}
