/**
 * @file
 * @brief This file contains the implementation of class Writer.
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
#include "Writer.h"
#include "utils/AIAlert.h"
#include <iostream>

namespace xml {

Header::Header(std::ostream& os) : m_os(os)
{
  os << "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\" ?>\n";
  if (os.bad())
  {
    THROW_MALERT("Failed to write xml header.");
  }
}

Header::~Header()
{
}

} // namespace xml
