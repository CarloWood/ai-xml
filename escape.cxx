/**
 * @file
 * @brief This file contains the implementation of xml escape functions.
 *
 * Copyright (C) 2014, 2016  Carlo Wood.
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
#include <string>

namespace xml {

std::string escape(std::string const& str)
{
  std::string result;
  for (std::string::const_iterator iter = str.begin(); iter != str.end(); ++iter)
  {
    char const c = *iter;
    switch (c)
    {
      case '<':
	result.append("&lt;");
	break;
      case '>':
	result.append("&gt;");
	break;
      case '&':
	result.append("&amp;");
	break;
      case '"':
        result.append("&quot;");
	break;
      case '\'':
	result.append("&apos;");
	break;
      default:
	result.push_back(c);
	break;
    }
  }
  return result;
}

// Replace '--' with '- -', see http://en.wikipedia.org/wiki/XML#Comments
std::string escape_comment(std::string const& comment)
{
  std::string result = comment;
  std::string::size_type off = std::string::npos;
  while ((off = result.rfind("--", off)) != std::string::npos)
  {
    result.replace(off, 2, "- -");
  }
  return result;
}

} // namespace xml
