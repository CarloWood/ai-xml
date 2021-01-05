/**
 * @file
 * @brief This file contains the implementation of class ReadBridge.
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
#include "Reader.h"
#include "utils/AIAlert.h"
#include "debug.h"
#include <libxml++/libxml++.h>
#include <libxml++/nodes/element.h>

namespace xml {

void ReadBridge::state_type::refresh_children(Glib::ustring const& name)
{
  m_current_parent = m_element;
  m_current_child_name = name;
  m_child_list = m_current_parent->get_children(m_current_child_name);
  m_current_child = m_child_list.begin();
}

void ReadBridge::state_type::get_element()
{
  while (m_current_child != m_child_list.end())
  {
    xmlpp::Element const* node = dynamic_cast<xmlpp::Element const*>(*m_current_child);
    if (node)
    {
      m_element = node;
      return;
    }
    ++m_current_child;
  }
  THROW_ALERT_CLASS(NoChildLeft, "While processing children of element <[PARENT]> (line [LINE]): no child with name <[NAME]> (left)!",
      AIArgs("[PARENT]", m_current_parent->get_name())("[LINE]", m_current_parent->get_line())("[NAME]", m_current_child_name));
}

void ReadBridge::get_element()
{
  return m_state.get_element();
}

void ReadBridge::state_type::node_name(char const* name_ptr, xmlpp::Element const* root_element)
{
  // Avoid re-constructing a Glib::ustring several times.
  Glib::ustring const name(name_ptr);

  if (!m_element)
  {
    // If this fails then you didn't parse a document. Call Reader::parse().
    ASSERT(root_element);

    m_element = root_element;
    if (m_element->get_name() != name)
    {
      THROW_ALERT("Root node has name <[ROOTNAME]>, expected <[NAME]>.",
	  AIArgs("[ROOTNAME]", m_element->get_name())("[NAME]", name));
    }
    Dout(dc::xmlparser, "Found root node <" << m_element->get_name() << ">.");
  }
  else
  {
    if (m_element->get_parent() != m_current_parent || m_current_child_name != name)
    {
      refresh_children(name);
    }
    else if (m_current_child != m_child_list.end())
    {
      ++m_current_child;
    }
    get_element();
    m_element = dynamic_cast<xmlpp::Element const*>(*m_current_child);
    Dout(dc::xmlparser, "Starting element <" << m_element->get_name() << "> line " << m_element->get_line() << ".");
  }
}

void ReadBridge::attribute(char const* name, char const* value)
{
  xmlpp::Element const* element = m_state.m_element;
  xmlpp::Attribute const* attribute = element->get_attribute(name);
  if (!attribute)
  {
    THROW_ALERT("Element <[ELEMENTNAME]> at line [LINE] does not have attribute '[NAME]'.",
	AIArgs("[ELEMENTNAME]", element->get_name())("[LINE]", element->get_line())("[NAME]", name));
  }
  if (attribute->get_value() != value)
  {
    THROW_ALERT("Wrong attribute '[NAME]' in node <[ELEMENTNAME]> (line [LINE]); value is \"[VALUE]\", expected \"[EXPECTED]\".",
	AIArgs("[NAME]", name)("[ELEMENTNAME]", element->get_name())("[LINE]", element->get_line())("[VALUE]", attribute->get_value())("[EXPECTED]", value));
  }
  Dout(dc::xmlparser, "Found attribute '" << attribute->get_name() << "=\"" << attribute->get_value() << "\"'.");
}

void ReadBridge::child(char const* name, char const* value)
{
  open_child(name);
  if (value != read_child_stream())
  {
    xmlpp::Element const* element = dynamic_cast<xmlpp::Element const*>(*m_state.m_current_child);
    THROW_ALERT("Element <[NAME]> (line [LINE]) has value \"[VALUE]\", expected \"[EXPECTED]\".",
	AIArgs("[NAME]", name)("[LINE]", element->get_line())("[VALUE]", read_child_stream())("[EXPECTED]", value));
  }
  close_child();
}

void ReadBridge::state_type::swap(state_type& state)
{
  std::swap(m_current_parent, state.m_current_parent);
  std::swap(m_current_child_name, state.m_current_child_name);
  m_child_list.swap(state.m_child_list);
  std::swap(m_current_child, state.m_current_child);
  std::swap(m_element, state.m_element);
}

void ReadBridge::open_child()
{
  DoutEntering(dc::xmlparser, "ReadBridge::open_child()");

  Bridge::push_state();
  state_type tmp_state(m_state.m_element);
  m_state_stack.push(tmp_state);
  m_state.swap(m_state_stack.top());
  m_state.m_current_child = m_state.m_child_list.end();
  Debug(libcw_do.push_marker());
  Debug(libcw_do.marker().append("| "));
}

void ReadBridge::open_child(char const* name)
{
  DoutEntering(dc::xmlparser, "ReadBridge::open_child(\"" << name << "\")");

  open_child();
  m_state.refresh_children(name);
  m_state.get_element();
}

void ReadBridge::next_child()
{
  // Call get_element() (open_child(name) or next_child()) before calling next_child().
  ASSERT(m_state.m_current_child != m_state.m_child_list.end());
  ++m_state.m_current_child;
  m_state.get_element();
}

bool ReadBridge::read_attribute(char const* name, std::string& attribute_str, bool mandatory) const
{
  xmlpp::Element const* element = m_state.m_element;
  xmlpp::Attribute const* attribute = element->get_attribute(name);
  if (!attribute)
  {
    if (mandatory)
    {
      THROW_ALERT("Element <[ELEMENTNAME]> at line [LINE] does not have attribute '[NAME]'.",
	  AIArgs("[ELEMENTNAME]", element->get_name())("[LINE]", element->get_line())("[NAME]", name));
    }
    return false;
  }
  attribute_str = attribute->get_value();
  Dout(dc::xmlparser, "Read attribute '" << attribute->get_name() << "=\"" << attribute->get_value() << "\"'.");
  return true;
}

std::string ReadBridge::read_child_stream()
{
  xmlpp::Element const* node = dynamic_cast<xmlpp::Element const*>(*m_state.m_current_child);
  // Call get_element() (open_child(name) or next_child()) before calling read_child_stream.
  ASSERT(node);
  xmlpp::TextNode const* textnode = node->get_first_child_text();
  if (!textnode)
  {
    Dout(dc::xmlparser, "ReadBridge::read_child_stream(): (xmlpp::Element*)\"" << node->get_name() << "\") has no text node. Returning empty string.");
    return "";
  }
  return textnode->get_content();
}

void ReadBridge::close_child()
{
  Debug(libcw_do.pop_marker());
  Dout(dc::xmlparser, "ReadBridge::close_child() </" << m_state.m_current_child_name << ">");
  m_state.swap(m_state_stack.top());
  m_state_stack.pop();
  Bridge::pop_state();
}

} // namespace xml
