/**
 * \file read_from_stream.h
 * \brief This file contains the declarations of template function read_from_stream.
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

#ifndef XML_READ_FROM_STREAM_H
#define XML_READ_FROM_STREAM_H

#include "set_locale_for.h"

#ifndef USE_PCH
#include <iostream>
#include <iomanip>
#include <vector>
#endif

namespace xml {

/// @defgroup read_from_stream read_from_stream
/// \brief Deserialize an element of type T for reading from XML.
/// @{

/**
  * \brief Deserialize an element of type T for reading from XML.
  *
  * The default just uses the operator>> that is defined for the element, with whitespace skipping.
  * Use specialization for classes that need something else for XML.
  */
template<typename T>
void read_from_stream(std::istream& is, T& var)
{
  set_locale_for<T>::for_stream(is, var);
  is >> std::boolalpha >> std::skipws >> var;
}

/// @defgroup read_from_stream_specializations Specializations
/// \brief Specializations of template functions read_from_stream.
/// @{

/**
  * \brief Specialization for `std::string`.
  *
  * This functions reads the <em>whole</em> stream, including any whitespace, into \a var.
  */
template<>
void read_from_stream(std::istream& is, std::string& var);

/**
  * \brief Specialization for `std::vector<T>`.
  *
  * This functions reads 'T' values from the stream (using read_from_stream) and adds them to
  * the end of the vector until eof happens on the stream. The vector is cleared first.
  */
template<typename T>
void read_from_stream(std::istream& is, std::vector<T>& vector)
{
  while (is.good())
  {
    T val;
    try
    {
      read_from_stream(is, val);
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
}

/**
  * \brief Specialization for `std::vector<std::string>`.
  *
  * This functions reads white space separated string values from the stream and adds them to
  * the end of the vector until eof happens on the stream. The vector is cleared first.
  */
template<>
void read_from_stream(std::istream& is, std::vector<std::string>& vector);

/// @}
/// @}

} // namespace xml

#endif // XML_READ_FROM_STREAM_H
