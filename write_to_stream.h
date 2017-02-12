/**
 * @file
 * @brief This file contains the declaration of template function write_to_stream.
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

#pragma once

#include "set_locale_for.h"

#include <iostream>
#include <iomanip>
#include <vector>

namespace xml {

/// @defgroup write_to_stream write_to_stream
/// \brief Serialize an element of type T for reading from XML.
/// @{

/**
  * \brief Serialize an element of type T for reading from XML.
  *
  * The default just uses the operator<< that is defined for the element.
  * Use specialization for classes that need something else for XML.
  */
template<typename T>
void write_to_stream(std::ostream& os, T const& var)
{
  set_locale_for<T>::for_stream(os, var);
  os << std::boolalpha << var;
}

/// @}

/// @defgroup write_to_stream_specializations Specializations
/// \brief Specializations of template functions write_to_stream.
/// @{

/**
  * \brief Specialization for `float`.
  *
  * This functions writes a float floating-point value with a precision of six decimal places.
  */
template<>
void write_to_stream<double>(std::ostream& os, double const& val);

/**
  * \brief Specialization for `double`.
  *
  * This functions writes a double floating-point value with a precision that varies on it's value.
  * The precision is normally size decimal places after the decimal point, but when if the
  * value is smaller than 0.1 then the precision is increased to keep six significant digits
  * up to a maximum precision of nine for values less than 0.001 (ie, 0.000999999).
  */
template<>
void write_to_stream<double>(std::ostream& os, double const& val);

/**
  * \brief Specialization for `std::vector<T>`.
  *
  * This functions writes the elements of the vector, using write_to_stream, space separated to the stream.
  */
template<typename T>
void write_to_stream(std::ostream& os, std::vector<T> const& vector)
{
  if (vector.empty()) return;
  auto iter = vector.begin();
  for(;;)
  {
    write_to_stream(os, *iter);
    if (++iter == vector.end())
      break;
    os << ' ';
  }
}

/// @}
/// @}

} // namespae xml
