/**
 * @file
 * @brief This file contains the implementation of template class read_from_string.
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

#include "read_from_string.h"
#include "utils/AIAlert.h"
#include <limits>
#include <cstdio>

namespace xml {

template<typename T>
T read_integer(char const* type, std::string const& str)
{
  long long int result;
  sscanf(str.c_str(),"%lld", &result);
  if (result < (std::numeric_limits<T>::min)() || result > (std::numeric_limits<T>::max)())
  {
    THROW_MALERT("Invalid [TYPE] [VALUE]", AIArgs("[TYPE]", type)("[VALUE]", str));
  }
  return result;
}

template<>
void read_from_string(uint8_t& out, std::string const& str)
{
  out = read_integer<uint8_t>("uint8_t", str);
}

template<>
void read_from_string(int8_t& out, std::string const& str)
{
  out = read_integer<int8_t>("int8_t", str);
}

template<>
void read_from_string(uint16_t& out, std::string const& str)
{
  out = read_integer<uint16_t>("uint16_t", str);
}

template<>
void read_from_string(int16_t& out, std::string const& str)
{
  out = read_integer<int16_t>("int16_t", str);
}

template<>
void read_from_string(uint32_t& out, std::string const& str)
{
  out = read_integer<uint32_t>("uint32_t", str);
}

template<>
void read_from_string(int32_t& out, std::string const& str)
{
  out = read_integer<int32_t>("int32_t", str);
}

double read_float(std::string const& str)
{
  double result;
  sscanf(str.c_str(),"%lf", &result);
  return result;
}

template<>
void read_from_string(float& out, std::string const& str)
{
  out = read_float(str);
}

template<>
void read_from_string(double& out, std::string const& str)
{
  out = read_float(str);
}

template<>
void read_from_string(bool& out, std::string const& str)
{
  if (str == "true" || str == "1")
  {
    out = true;
    return;
  }
  else if (str != "false" && str != "0")
  {
    THROW_FMALERT("Invalid boolean [VALUE]", AIArgs("[VALUE]", str));
  }
  out = false;
}

} // namespace xml
