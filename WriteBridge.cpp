/**
 * \file WriteBridge.cpp
 * \brief This file contains the implementation of class WriteBridge.
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

#ifndef USE_PCH
#include "sys.h"
#endif

#include "utils/AIAlert.h"

#include "WriteBridge.h"
#include "escape.h"

#ifndef USE_PCH
#include <iostream>
#endif

namespace xml {

WriteBridge::WriteBridge(std::ostream& os, uint32_t version_major) : Bridge(version_major), m_os(os), m_state(closed)
{
}

void WriteBridge::node_name(char const* name)
{
  DoutEntering(dc::xmlparser, "WriteBridge::node_name(\"" << name << "\")");
  ASSERT(m_state.m_indentation >= 0);	// Call open_child() / close_child() around calling 'root.xml(parser)' for the root object.
  m_state.close_child(m_os);
  if (m_state.m_parent_tag_state == half_open)
  {
    m_os << ">\n";
    if (!m_os.good())
    {
      THROW_FALERT("Failed to write \"[DATA]\" to stream.",
	  AIArgs("[DATA]", ">\\n"));
    }
    m_state.m_parent_tag_state = open;
  }
  m_state.m_element_name = name;
  m_os << std::string(m_state.m_indentation, ' ') << '<' << m_state.m_element_name;
  if (!m_os.good())
  {
    THROW_FALERT("Failed to write \"[DATA]\" to stream.",
	AIArgs("[DATA]", "<" + m_state.m_element_name));
  }
  m_state.m_element_tag_state = half_open;
}

void WriteBridge::attribute(char const* name, char const* value)
{
  DoutEntering(dc::xmlparser, "WriteBridge::attribute(\"" << name << "\", \"" << value << "\")");
  std::string escaped_value = escape(value);
  m_os << ' ' << name << "=\"" << escaped_value << '"';
  if (!m_os.good())
  {
    THROW_FALERT("Failed to write \"[DATA]\" to stream.",
	AIArgs("[DATA]", std::string(" ") + name + "=\"" + escaped_value + "\""));
  }
}

void WriteBridge::child(char const* name, char const* value)
{
  DoutEntering(dc::xmlparser, "WriteBridge::child(\"" << name << "\", \"" << value << "\")");
  open_child(name);
  write_child_stream(value);
  close_child();
}

void WriteBridge::open_child()
{
  DoutEntering(dc::xmlparser, "WriteBridge::open_child()");

  Bridge::push_state();
  m_state_stack.push(m_state);
  m_state.m_parent_tag_state = m_state.m_element_tag_state;
  m_state.m_element_tag_state = closed;
  m_state.m_indentation += 2;
}

void WriteBridge::open_child(char const* name)
{
  DoutEntering(dc::xmlparser, "WriteBridge::open_child(\"" << name << "\")");

  open_child();
  m_state.m_element_name = name;
}

std::ostream& WriteBridge::get_os()
{
  return m_os;
}

void WriteBridge::write_child_stream(std::string const& element)
{
  DoutEntering(dc::xmlparser, "WriteBridge::write_child_stream(\"" << element << "\")");
  if (m_state.m_parent_tag_state == half_open)
  {
    ASSERT(m_state.m_element_tag_state == closed);
    m_os << ">\n";
    if (!m_os.good())
    {
      THROW_FALERT("Failed to write \"[DATA]\" to stream.",
	  AIArgs("[DATA]", ">\\n"));
    }
    m_state.m_parent_tag_state = open;
  }
  if (m_state.m_element_tag_state == half_open)
  {
    m_os << ">";
    if (!m_os.good())
    {
      THROW_FALERT("Failed to write \"[DATA]\" to stream.",
	  AIArgs("[DATA]", ">"));
    }
  }
  else if (m_state.m_element_tag_state == closed)
  {
    m_os << std::string(m_state.m_indentation, ' ') << "<" << m_state.m_element_name << ">";
    if (!m_os.good())
    {
      THROW_FALERT("Failed to write \"[DATA]\" to stream.",
	  AIArgs("[DATA]", "<" + m_state.m_element_name + ">"));
    }
  }
  m_os << escape(element) << "</" << m_state.m_element_name << ">\n";
  m_state.m_element_tag_state = closed;
}

void WriteBridge::write_attribute(char const* name, std::string const& attribute_str)
{
  std::string escaped_str = escape(attribute_str);
  m_os << ' ' << name << "=\"" << escaped_str << '"';
  if (!m_os.good())
  {
    THROW_FALERT("Failed to write \"[DATA]\" to stream.",
	AIArgs("[DATA]", std::string(" ") + name + "=\"" + escaped_str + '"'));
  }
}

void WriteBridge::state_type::close_child(std::ostream& os)
{
  if (m_element_tag_state == WriteBridge::half_open)
  {
    os << " />\n";
    if (!os.good())
    {
      THROW_FALERT("Failed to write \"[DATA]\" to stream.",
	  AIArgs("[DATA]", " />\\n"));
    }
  }
  else if (m_element_tag_state == WriteBridge::open)
  {
    os << std::string(m_indentation, ' ') << "</" << m_element_name << ">\n";
    if (!os.good())
    {
      THROW_FALERT("Failed to write \"[DATA]\" to stream.",
          AIArgs("[DATA]", std::string(m_indentation, ' ') + "</" + m_element_name + ">\\n"));
    }
  }
  m_element_tag_state = closed;
}

void WriteBridge::close_child()
{
  DoutEntering(dc::xmlparser, "WriteBridge::close_child()");
  m_state.close_child(m_os);
  tag_state_type parent_tag_state = m_state.m_parent_tag_state;
  m_state = m_state_stack.top();
  m_state.m_element_tag_state = parent_tag_state;
  m_state_stack.pop();
  Bridge::pop_state();
}

} // namespace xml
