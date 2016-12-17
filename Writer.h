/**
 * \file Writer.h
 * \brief This file contains the declaration of class Writer.
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
 *
 * \class xml::Writer
 * \brief Writes an XML structure to a `std::ostream`.
 *
 * See class Bridge for a detailed description and example code.
 */

#ifndef XML_ROOTELEMENT_H
#define XML_ROOTELEMENT_H

#include "WriteBridge.h"

#ifndef USE_PCH
#include <iosfwd>
#endif

namespace xml {

/**
  * \brief Helper class
  *
  * This class writes out the XML header <em>before</em>
  * the constructor of WriteBridge is called which causes
  * the whole XML contents to be written.
  */
class Header
{
/// @cond Doxygen_Suppress
  protected:
    std::ostream& m_os;
    Header(std::ostream& os);
    virtual ~Header();
/// @endcond
};

class Writer : public Header, public WriteBridge
{
  public:

    /// Construct a Writer that can be used to write to an `std::ostream`.
    Writer(std::ostream& os) : Header(os), WriteBridge(os, 1) { }

    /**
      * \brief Write \a object as XML to the underlaying ostream.
      *
      * \param object : An object of a class type that implements void xml(xml::Bridge&).
      * Because the member function xml(xml::Bridge&) is not const (it is also used
      * to read into the object), \a object cannot be const.
      */
    template<typename T>
      void write(T& object);
};

template<typename T>
void Writer::write(T& object)
{
  open_child();
  object.xml(*this);
  close_child();
}

} // namespace xml

#endif // XML_ROOTELEMENT_H
