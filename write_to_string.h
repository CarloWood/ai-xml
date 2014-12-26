/**
 * \file write_to_string.h
 * \brief This file contains the declarations of template function write_to_string.
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

#ifndef XML_WRITE_TO_STRING_H
#define XML_WRITE_TO_STRING_H

#ifndef USE_PCH
#include <sstream>
#include <iomanip>
#endif

namespace xml {

/// @defgroup write_to_string write_to_string
/// \brief Function template to convert an object of arbitrary type to an unescaped XML string.
/// @{

/**
  * \brief Function template to convert an object of arbitrary type to an unescaped XML string.
  *
  * The default writes \a obj to a `std::ostringstream` using `operator<<` and returns the string.
  * Use specialization for classes that need something else for XML.
  */
template<typename T>
std::string write_to_string(T const& obj)
{
  std::ostringstream str;
  str.flags(std::ios::boolalpha);
  str << obj;			// error: cannot bind 'std::basic_ostream<char>' lvalue to 'std::basic_ostream<char>&&'
				// means that there is no std::ostream& operator<<(std::ostream&, T const&) declared.
				// Either define that, or specialize a xml::write_to_string for that type.
  return str.str();
}

/// @}

} // namespace xml

#endif // XML_WRITE_TO_STRING_H
