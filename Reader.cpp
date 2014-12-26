/**
 * \file Reader.cpp
 * \brief This file contains the implementation of class Reader.
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
#include "Reader.h"

#ifndef USE_PCH
#include <boost/filesystem/fstream.hpp>
#endif

namespace fs = boost::filesystem;

namespace xml {

Reader::Reader(void)
{
  m_parser.set_substitute_entities();
}

void Reader::parse(std::istream& file, uint32_t version_major)
{
  set_version(version_major);
  try
  {
    m_parser.parse_stream(file);
    if (!m_parser)
    {
      THROW_ALERT("Failed to parse XML: unknown error.");
    }
  }
  catch (xmlpp::parse_error const& error)
  {
    THROW_ALERT("Failed to parse XML: [WHAT]", AIArgs("[WHAT]", error.what()));
  }
  m_root_element = m_parser.get_document()->get_root_node();
}

void Reader::parse(fs::path const& filepath, uint32_t version_major)
{
  fs::ifstream file;
  try
  {
    if (!exists(filepath))
    {
      using namespace boost::system;
      throw fs::filesystem_error("", filepath, "", error_code(errc::no_such_file_or_directory, generic_category()));
    }
    file.open(filepath, std::ios_base::binary);
  }
  catch (fs::filesystem_error const& ex)
  {
    THROW_ALERT("fs::open: [ERROR]", AIArgs("[ERROR]", ex.what()));
  }

  std::cout << "Reading file " << filepath << "." << std::endl;

  parse(file, version_major);
  file.close();
}

} // namespace xml
