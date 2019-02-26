/**
 * @file
 * @brief This file contains the implementation of class Bridge.
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
#include "ReadBridge.h"
#include "debug.h"

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

void Bridge::push_state()
{
  m_state_stack.push(m_state);
  ++m_state.m_depth;
}

void Bridge::pop_state()
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

void Bridge::get_element()
{
  DoutFatal(dc::core, "Calling WriteBridge::get_element()!?");
}

void Bridge::next_child()
{
  DoutFatal(dc::core, "Calling WriteBridge::next_child()!?");
}

bool Bridge::read_attribute(char const* UNUSED_ARG(name), std::string& UNUSED_ARG(attribute_str), bool UNUSED_ARG(mandatory)) const
{
  DoutFatal(dc::core, "Calling WriteBridge::read_attribute()!?");
}

std::string Bridge::read_child_stream()
{
  DoutFatal(dc::core, "Calling WriteBridge::read_child_stream()!?");
}

// Virtual functions only implemented in WriteBridge:

void Bridge::write_attribute(char const* UNUSED_ARG(name), std::string const& UNUSED_ARG(raw_attribute))
{
  DoutFatal(dc::core, "Calling ReadBridge::write_attribute()!?");
}

std::ostream& Bridge::get_os()
{
  DoutFatal(dc::core, "Calling ReadBridge::get_os()!?");
}

void Bridge::write_child_stream(std::string const& UNUSED_ARG(var))
{
  DoutFatal(dc::core, "Calling ReadBridge::write_child_stream()!?");
}

} // namespace xml

#ifdef CWDEBUG
NAMESPACE_DEBUG_CHANNELS_START
channel_ct xmlparser("XMLPARSER");
NAMESPACE_DEBUG_CHANNELS_END
#endif
