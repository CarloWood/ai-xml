/**
 * \file ReadBridge.h
 * \brief This file contains the declaration of class ReadBridge.
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
 * \class xml::ReadBridge
 * \brief Parses an XML element.
 *
 * This class is a wrapper around xmlpp::DomParser and,
 * being derived from ReadBridge, represents the
 * root node of an XML structure.
 */

#ifndef XML_ELEMENTPARSER_H
#define XML_ELEMENTPARSER_H

#include "Bridge.h"

#ifndef USE_PCH
#include <iosfwd>
#include <string>
#include <stack>
#endif

namespace xmlpp {
class Element;
} // namespace xmlpp

namespace xml {

class ReadBridge : public Bridge
{
  public:
    /**
      * \brief Type of m_state.
      *
      * As soon a the name of a new child element is known,
      * either by calling node_name() from it's xml(Bridge&) method
      * or when open_child(name) is called, the name is stored in
      * m_current_child_name, the child is looked up in m_element,
      * which is stored in m_current_parent, all children with
      * that name are loaded into m_child_list and m_current_child
      * is set to point to the first child.
      *
      * On subsequent calls to node_name() with the same name
      * and parent, only m_current_child is advanced until there
      * are not children left with that name (causing the exception
      * NoChildLeft to be thrown).
      */
    struct state_type {
      xmlpp::Element const* m_current_parent;			///< The parent element of the list in m_child_list.
      Glib::ustring m_current_child_name;			///< The name of the child elements in m_child_list.
      xmlpp::Node::NodeList m_child_list;			///< A list with child nodes of m_current_parent with name m_current_child_name.
      xmlpp::Node::NodeList::const_iterator m_current_child;	///< A pointer to the current child in m_child_list.
      xmlpp::Element const* m_element;				///< The current (child) element being processed.

      /// Construct an initial state_type suitable for the root element.
      state_type() : m_current_parent(NULL), m_current_child(m_child_list.end()), m_element(NULL) { }
      /// Construct a state_type for element \a element with no children loaded yet.
      state_type(xmlpp::Element const* element) : m_current_parent(NULL), m_current_child(m_child_list.end()), m_element(element) { }

      void swap(state_type& state);				///< Swap the contents with \a state, preventing a copy of m_child_list and therefore keeping iterators to it valid.
      void refresh_children(Glib::ustring const& name);		///< Load children with name \a name.
      void get_element();					///< If m_current_child does not point to an xmlpp::Element, advance it till it does (or reaches the end of the list).
      /**
        * \brief Initialize the state data.
	*
	* \param name_ptr : the name of the (child) element we want to use.
	* \param root_element : a pointer to the root element. Only used when this is the first time this function is called.
	*
	* On the first call, m_element is set to \a root_element.
	* Otherwise, on the first call with this name, m_element is set to the first child of the current element.
	* On subsequent calls with that name, m_element is advanced to the next child with that name.
	*
	* \throws NoChildLeft when no (subsequent) child with that name exists.
	*/
      void node_name(char const* name_ptr, xmlpp::Element const* root_element);
    };

  protected:
    xmlpp::Element const* m_root_element;			///< The root element of the document.
    state_type m_state;						///< State information.
    std::stack<state_type> m_state_stack;			///< Stored state information of parent elements.

  public:
    /// Return the internal state of the ReadBridge.
    state_type const& state() const { return m_state; }

  protected:
    /// Construct an uninitialized ReadBridge.
    ReadBridge() : Bridge(0), m_root_element(NULL) { }

    /*virtual*/ bool writing() const { return false; }
    /*virtual*/ void node_name(char const* name) { m_state.node_name(name, m_root_element); }
    /*virtual*/ void attribute(char const* name, char const* value);
    /*virtual*/ void child(const char*, const char*);

/// @cond Doxygen_Suppress
  protected:
    /*virtual*/ void open_child();
    /*virtual*/ void open_child(char const* name);
    /*virtual*/ void close_child();
    /*virtual*/ void get_element();
    /*virtual*/ void next_child();
    /*virtual*/ bool read_attribute(char const* name, std::string& attribute_str, bool mandatory) const;
    /*virtual*/ std::string read_child_stream();
/// @endcond
};

} // namespace xml

#endif // XML_ELEMENTPARSER_H
