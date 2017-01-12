/**
 * @file
 * @brief This file contains the declarations of template function read_from_string.
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

#pragma once

#include <string>
#include <new>

namespace xml {

/// @defgroup read_from_string read_from_string
/// \brief Function template to convert unescaped XML string to an object of arbitrary type.
/// @{

/**
  * \brief Function template to convert unescaped XML string to an object of arbitrary type.
  *
  * The default constructs \a obj from \a str using placement-new.
  * Use specialization for classes that need something else for XML.
  */
template<typename T>
void read_from_string(T& obj, std::string const& str)
{
  // Construct in-place with placement new.
  new (&obj) T(str);
}

/// @defgroup read_from_string_specializations Specializations
/// \brief Specializations of template functions read_from_string.
/// @{

/// \brief Specialization for `uint8_t`.
template<>
void read_from_string(uint8_t& out, std::string const& str);

/// \brief Specialization for `int8_t`.
template<>
void read_from_string(int8_t& out, std::string const& str);

/// \brief Specialization for `uint16_t`.
template<>
void read_from_string(uint16_t& out, std::string const& str);

/// \brief Specialization for `int16_t`.
template<>
void read_from_string(int16_t& out, std::string const& str);

/// \brief Specialization for `uint32_t`.
template<>
void read_from_string(uint32_t& out, std::string const& str);

/// \brief Specialization for `int32_t`.
template<>
void read_from_string(int32_t& out, std::string const& str);

/// \brief Specialization for `float`.
template<>
void read_from_string(float& out, std::string const& str);

/// \brief Specialization for `double`.
template<>
void read_from_string(double& out, std::string const& str);

/// \brief Specialization for `std::string`.
template<>
void read_from_string(bool& out, std::string const& str);

/// @}
/// @}

} // namespace xml
