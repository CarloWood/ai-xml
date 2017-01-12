/**
 * @file
 * @brief This file contains the declaration of class Reader.
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
 *
 * \class xml::Reader
 * \brief Class to read XML files.
 */

#pragma once

#include "ReadBridge.h"

#include <cinttypes>
#include <iosfwd>
#include <boost/filesystem.hpp>
#include <libxml++/libxml++.h>

namespace xml {

class Reader : public ReadBridge
{
  private:
    xmlpp::DomParser m_parser;

  public:
    /// Construct an empty XML parser.
    Reader();

    /// Parse an XML file.
    void parse(std::istream& file, uint32_t version_major);

    /// Parse an XML file.
    void parse(boost::filesystem::path const& file, uint32_t version_major);
};

} // namespace xml
