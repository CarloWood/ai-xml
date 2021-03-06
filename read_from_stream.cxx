/**
 * @file
 * @brief This file contains the implementation of template function read_from_stream.
 *
 * Copyright (C) 2014 - 2017  Carlo Wood.
 *
 * RSA-1024 0x624ACAD5 1997-01-26                    Sign & Encrypt
 * Fingerprint16 = 32 EC A7 B6 AC DB 65 A6  F6 F6 55 DD 1C DC FF 61
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

#include "sys.h"
#include "read_from_stream.h"
#include <iostream>

namespace xml {

template<>
void read_from_stream(std::istream& is, std::string& var)
{
  var.clear();
  if (is.good())
  {
    std::string::size_type len = 0;
    std::streambuf* sbuf = is.rdbuf();
    std::streamsize size;
    for(;;)
    {
      while ((size = sbuf->in_avail()) > 0)
      {
	var.resize(len + size);
	sbuf->sgetn(&var[len], size);
	len += size;
      }
      if (!is.eof())
      {
	char c;
	is >> c;
	if (!is.eof())
	{
	  sbuf->sungetc();
	  continue;
	}
	is.clear(std::ios::eofbit);
      }
      break;
    }
  }
}

template<>
void read_from_stream<std::string>(std::istream& is, std::vector<std::string>& vector)
{
  std::ios_base::fmtflags oldflags = is.flags(std::ios::skipws);
  while (is.good())
  {
    std::string val;
    try
    {
      is >> val;
    }
    catch(std::exception const&)
    {
      break;
    }
    if (is.fail())
    {
      break;
    }
    vector.push_back(val);
  }
  is.clear();
  is.flags(oldflags);
}

} // namespace xml
