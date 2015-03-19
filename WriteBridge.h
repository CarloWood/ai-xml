/**
 * \file WriteBridge.h
 * \brief This file contains the declaration of class WriteBridge.
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
 *
 * \class xml::WriteBridge
 * \brief Writes an XML element to a `std::ostream`.
 *
 * See class Bridge for a detailed description and example code.
 */

#ifndef XML_ELEMENT_H
#define XML_ELEMENT_H

#include "Bridge.h"

#ifndef USE_PCH
#include <cinttypes>
#include <iosfwd>
#include <string>
#include <stack>
#endif

namespace xmlpp {
class WriteBridge;
} // namespace xmlpp

namespace xml {

class Writer;

class WriteBridge : public Bridge
{
  private:
    std::ostream& m_os;
    enum tag_state_type { closed, half_open, open };
    struct state_type {
      int m_indentation;
      tag_state_type m_parent_tag_state;
      std::string m_element_name;
      tag_state_type m_element_tag_state;

      state_type(tag_state_type parent_tag_state) : m_indentation(-2), m_parent_tag_state(parent_tag_state), m_element_tag_state(closed) { }
      void close_child(std::ostream& os);
    };
    state_type m_state;
    std::stack<state_type> m_state_stack;

  public:
    /**
      * \brief Construct a WriteBridge.
      *
      * \param os : the `std::ostream` to write to.
      * \param version_major : use this as version (returned by Bridge::version()) for this element and its children.
      */
    WriteBridge(std::ostream& os, uint32_t version_major);

    /*virtual*/ bool writing() const { return true; }
    /*virtual*/ void node_name(char const* name);
    /*virtual*/ void attribute(char const* name, char const* value);
    /*virtual*/ void child(const char*, const char*);

/// @cond Doxygen_Suppress
  protected:
    /*virtual*/ void open_child();
    /*virtual*/ void open_child(char const* name);
    /*virtual*/ void close_child();
    /*virtual*/ std::ostream& get_os();
    /*virtual*/ void write_attribute(char const* name, std::string const& raw_attribute);
    /*virtual*/ void write_child_stream(std::string const& element);
/// @endcond
};

} // namespace xml

#endif // XML_ELEMENT_H
