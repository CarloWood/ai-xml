/**
 * \file Bridge.cpp
 * \brief This file contains the implementation of class Bridge.
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

#include "ReadBridge.h"

#ifndef USE_PCH
#include "debug.h"
#endif

namespace xml {

void Bridge::set_version(uint32_t version_major)
{
#ifdef CWDEBUG
  ReadBridge* parser = dynamic_cast<ReadBridge*>(this);
  if (parser && m_state.m_version_major != version_major)
  {
    Dout(dc::xmlparser|continued_cf, "Version changed from " << m_state.m_version_major << " to " << version_major);
    if (parser->state().m_element)
    {
      Dout(dc::continued, " for element <" << parser->state().m_element->get_name() << ">");
    }
    Dout(dc::finish, ".");
  }
#endif
  m_state.m_version_major = version_major;
}

void Bridge::push_state(void)
{
  m_state_stack.push(m_state);
  ++m_state.m_depth;
}

void Bridge::pop_state(void)
{
#ifdef CWDEBUG
  uint32_t version = m_state.m_version_major;
#endif
  m_state = m_state_stack.top();
  m_state_stack.pop();
#ifdef CWDEBUG
  ReadBridge* parser = dynamic_cast<ReadBridge*>(this);
  if (parser && version != m_state.m_version_major)
  {
    Dout(dc::xmlparser|continued_cf, "Version restored to " << m_state.m_version_major << " (was " << version << ")");
    if (parser->state().m_element)
    {
      Dout(dc::continued, " upon reentering element <" << parser->state().m_element->get_name() << ">");
    }
    Dout(dc::finish, ".");
  }
#endif
}

// Virtual functions only implemented in ReadBridge:

void Bridge::get_element(void)
{
  DoutFatal(dc::core, "Calling WriteBridge::get_element()!?");
}

void Bridge::next_child(void)
{
  DoutFatal(dc::core, "Calling WriteBridge::next_child()!?");
}

bool Bridge::read_attribute(char const* UNUSED_ARG(name), std::string& UNUSED_ARG(attribute_str), bool UNUSED_ARG(mandatory)) const
{
  DoutFatal(dc::core, "Calling WriteBridge::read_attribute()!?");
}

std::string Bridge::read_child_stream(void)
{
  DoutFatal(dc::core, "Calling WriteBridge::read_child_stream()!?");
}

// Virtual functions only implemented in WriteBridge:

void Bridge::write_attribute(char const* UNUSED_ARG(name), std::string const& UNUSED_ARG(raw_attribute))
{
  DoutFatal(dc::core, "Calling ReadBridge::write_attribute()!?");
}

std::ostream& Bridge::get_os(void)
{
  DoutFatal(dc::core, "Calling ReadBridge::get_os()!?");
}

void Bridge::write_child_stream(std::string const& UNUSED_ARG(var))
{
  DoutFatal(dc::core, "Calling ReadBridge::write_child_stream()!?");
}

} // namespace xml
