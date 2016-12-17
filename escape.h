/**
 * \file escape.h
 * \brief This file contains the declarations of xml escape functions.
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

#ifndef XML_ESCAPE_H
#define XML_ESCAPE_H

#ifndef USE_PCH
#include <string>
#endif

namespace xml {

/**
  * \brief Returns \a str using XML escapes.
  *
  * - '<' --> "&lt;"
  * - '>' --> "&gt;"
  * - '&' --> "&amp;"
  * - '"' --> "&quot;"
  * - '\'' --> "&apos;"
  */
std::string escape(std::string const& str);

/**
  * \brief Returns \a comment, 'escaped' in a way that XML will not interpret it anymore as XML comment.
  */
std::string escape_comment(std::string const& comment);

} // namespace xml

#endif // XML_ESCAPE_H
