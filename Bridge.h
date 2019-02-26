/**
 * @file
 * @brief Definition of class Bridge.
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
 * \class xml::Bridge
 *
 * \brief Acts as a bridge between custom classes and reading / writing xml files.
 *
 * The Bridge class is used to both read and write xml files.
 * Classes that want to support importing from and/or exporting to xml files
 * should implement the method `void xml(xml::Bridge& xml)`.
 *
 * XML (de)serialization works by providing classes, one per element type,
 * which provide a public method `void xml(xml::Bridge& xml)`.
 * Each such method must start with a call to
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xml.node_name("name_of_the_element")
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * see the example code below.
 *
 * Below that, first all attributes are listed with calls to
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xml.attribute("name_of_attribute", m_variable)
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * where `m_variable` is a class member that stores the attribute.
 *
 * If the attribute is optional, use
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xml.optional_attribute("name_of_attribute", m_variable[, default_value[, true]])
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * where the default `default_value` is default constructed variable,
 * and the default last parameter (`always_write`) is `false`.
 * Passing `true` will cause this attribute to be written out regardless
 * of it being equal to the default value.
 *
 * Next there is the option to read the contents of the current element
 * as text (using read_from_stream() and write_to_stream()), using:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xml.text_stream(m_some_var);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * where `m_some_var` is a class member (ie, a `std::stream`).
 *
 * Finally all children, if any are listed.
 *
 * There are two main types of calls to hook in child nodes of an element:
 * 1. `child`
 * 2. `children`
 *
 * The first can be used to encode a single, mandatory child element.
 * For example,
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xml.child(m_some_var);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * where `m_some_var` is a class member that in turn needs to have
 * a `void xml(xml::Bridge& xml)` member.
 *
 * The second version is for containers of just classes.
 * For example,
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xml.children(m_some_vector);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * where `m_some_vector` is a std::vector<SomeClass>, or other
 * standard container, that implements the `push_back` method,
 * and `SomeClass` in turn needs to have a `void xml(xml::Bridge& xml)` member.
 * Or, for a container that implements the `insert` method instead,
 * you'd do:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xml.children(m_some_vector, xml::insert);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Both methods have a `_stream' variant that take the name of the element
 * as extra (first) parameter. For example:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * xml.child_stream("name_of_child_element", m_some_var);
 * xml.children_stream("name_of_child_element", m_some_vector);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * in which case `m_some_var` and the elements of `m_some_vector` do
 * not need to be a class with an `xml()` method, but can be any
 * type that will be read from or written to by a call to
 * read_from_stream() or write_to_stream() respectively, which are
 * template functions that by default just use `operator>>(istream)`
 * and `operator<<(ostream)`, but which can be specialized for
 * any type. In fact, read_from_stream() is already specialized
 * for `std::string` to make it read all characters, including
 * whitespace.
 *
 * Example Code
 * ------------
 *
 * Suppose you want to process the following xml file,
 * which was randomly picked from http://www.service-architecture.com/articles/object-oriented-databases/complex_data.html:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * <catalog>
 *    <product description="Cardigan Sweater" product_image="cardigan.jpg">
 *       <catalog_item gender="Men's">
 *          <item_number>QWZ5671</item_number>
 *          <price>39.95</price>
 *          <size description="Medium">
 *             <color_swatch image="red_cardigan.jpg">Red</color_swatch>
 *             <color_swatch image="burgundy_cardigan.jpg">Burgundy</color_swatch>
 *          </size>
 *          <size description="Large">
 *             <color_swatch image="red_cardigan.jpg">Red</color_swatch>
 *             <color_swatch image="burgundy_cardigan.jpg">Burgundy</color_swatch>
 *          </size>
 *       </catalog_item>
 *       <catalog_item gender="Women's">
 *          <item_number>RRX9856</item_number>
 *          <price>42.50</price>
 *          <size description="Small">
 *             <color_swatch image="red_cardigan.jpg">Red</color_swatch>
 *             <color_swatch image="navy_cardigan.jpg">Navy</color_swatch>
 *             <color_swatch image="burgundy_cardigan.jpg">Burgundy</color_swatch>
 *          </size>
 *          <size description="Medium">
 *             <color_swatch image="red_cardigan.jpg">Red</color_swatch>
 *             <color_swatch image="navy_cardigan.jpg">Navy</color_swatch>
 *             <color_swatch image="burgundy_cardigan.jpg">Burgundy</color_swatch>
 *             <color_swatch image="black_cardigan.jpg">Black</color_swatch>
 *          </size>
 *          <size description="Large">
 *             <color_swatch image="navy_cardigan.jpg">Navy</color_swatch>
 *             <color_swatch image="black_cardigan.jpg">Black</color_swatch>
 *          </size>
 *          <size description="Extra Large">
 *             <color_swatch image="burgundy_cardigan.jpg">Burgundy</color_swatch>
 *             <color_swatch image="black_cardigan.jpg">Black</color_swatch>
 *          </size>
 *       </catalog_item>
 *    </product>
 * </catalog>
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This requires five classes:
 *
 * 1. Catalog
 * 2. Product
 * 3. CatalogItem
 * 4. Size
 * 5. ColorSwatch
 *
 * The elements &lt;item_number&gt; and &lt;price&gt; can be processed
 * as `string` and `float` respectively.
 *
 * Furthermore, suppose you want to store the ColorSwatch
 * objects as references to a `std::map` in the Product
 * object, so that no duplicates are stored.
 * The key for those objects is the text given (Red, Navy, etc)
 * while the remaining data (the `image` attribute) is stored
 * as data in the map.
 *
 * The first two classes then look like this:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * class Catalog {
 *   private:
 *     std::list<Product> m_products;
 *   public:
 *     void xml(xml::Bridge& xml);
 * };
 *
 * class Product {
 *   public:
 *     typedef std::map<std::string, std::string> image_map_type;
 *   private:
 *     std::string m_description;
 *     std::string m_product_image;
 *     image_map_type m_image_map;
 *     std::vector<CatalogItem> m_items;
 *   public:
 *     image_map_type::iterator insert(std::string const* key, std::string const* info)
 *     {
 *       return m_image_map.insert(image_map_type::value_type(*key, *info)).first;
 *     }
 *     void xml(xml::Bridge& xml);
 * };
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Where the `Catalog` contains a list of `Product`s,
 * and the `Product` has two attributes, 'description'
 * and 'product_image', and contains the map that maps
 * a product name to an image name, and a vector of
 * `CatalogItem`s.
 *
 * The following two classes are straightforward as well:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * class CatalogItem {
 *   private:
 *     std::string m_gender;
 *     std::string m_item_number;
 *     float m_price;
 *     std::vector<Size> m_sizes;
 *   public:
 *     void xml(xml::Bridge& xml);
 * };
 *
 * class Size {
 *   private:
 *     std::string m_description;
 *     std::vector<ColorSwatch> m_color_swatches;
 *   public:
 *     void xml(xml::Bridge& xml);
 * };
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Where the `ColorSwatch` is actually a reference to
 * the `std::map` in the parent `Product`:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * class ColorSwatch {
 *   private:
 *     std::map<std::string, std::string>::iterator m_ref;
 *   public:
 *     void xml(xml::Bridge& xml);
 * };
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Then, to actually parse this XML file into these objects
 * the following methods need to be implemented:
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * void Catalog::xml(xml::Bridge& xml)
 * {
 *   xml.node_name("catalog");
 *   xml.children(m_products);
 * }
 *
 * void Product::xml(xml::Bridge& xml)
 * {
 *   xml.node_name("product");
 *   xml.attribute("description", m_description);
 *   xml.attribute("product_image", m_product_image);
 *   xml.set_user_ptr(this);
 *   xml.children(m_items);
 * }
 *
 * void CatalogItem::xml(xml::Bridge& xml)
 * {
 *   xml.node_name("catalog_item");
 *   xml.attribute("gender", m_gender);
 *   xml.child_stream("item_number", m_item_number);
 *   xml.child_stream("price", m_price);
 *   xml.children(m_sizes);
 * }
 *
 * void Size::xml(xml::Bridge& xml)
 * {
 *   xml.node_name("size");
 *   xml.attribute("description", m_description);
 *   xml.children(m_color_swatches);
 * }
 *
 * void ColorSwatch::xml(xml::Bridge& xml)
 * {
 *   xml.node_name("color_swatch");
 *   std::string* name;
 *   std::string* image;
 *   if (xml.writing())
 *   {
 *     name = &mref->first;
 *     image = &mref->second;
 *   }
 *   else
 *   {
 *     name = new std::string;
 *     image = new std::string;
 *   }
 *   xml.attribute("image", *image);
 *   xml.text_stream(*name);
 *   if (!writing())
 *   {
 *     Product* product = static_cast<Product*>(xml.get_user_ptr());
 *     m_ref = product->insert(name, image);
 *     delete image;
 *     delete name;
 *   }
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This above code is provided as complete test application in `src/xml` under the name `catalog_test.cpp`.
 */

#pragma once

#include "read_from_string.h"
#include "write_to_string.h"
#include "read_from_stream.h"
#include "write_to_stream.h"

#include "utils/AIAlert.h"
#include "debug.h"

#include <libxml++/libxml++.h>
#include <stack>

#if defined(CWDEBUG) && !defined(DOXYGEN)
NAMESPACE_DEBUG_CHANNELS_START
extern channel_ct xmlparser;
NAMESPACE_DEBUG_CHANNELS_END
#endif

/// \brief XML specific classes and functions.
namespace xml {

/// The return type of Bridge::attribute.
enum attribute_return_type {
  reading_default_attribute,		///< The attribute could not be found, the default value was returned.
  reading_attribute_success,		///< The attribute was successfully read.
  writing_attribute_success,		///< The attribute was successfulle written.
  writing_attribute_skipped		///< The attribate was not written because it was optional and its value was equal to the default value.
};

/// The return type of Bridge::text_stream and Bridge::child_stream.
enum element_return_type {
  reading_default_element,		///< The node/element could not be found, the default value was returned.
  reading_element_success,		///< The node/element was successfully read.
  writing_element_success,		///< The node/element was successfulle written.
  writing_element_skipped		///< The node/element was not written because it was optional and its value was equal to the default value.
};

/// Type that states we want to insert child elements in a container.
enum insert_method_type {
  insert
};

/// Type that states we want to push_back child elements in a container.
enum push_back_method_type {
  push_back
};

class Bridge
{
  protected:
    /** \class state_type
      * \brief The type of m_state.
      * \sa version, set_version, get_user_ptr, set_user_ptr
      * \internal
      *
      * This class is for internal use, it is of not concern to the user.
      */
    struct state_type {
      int m_depth;			///< Number of calls to open_child() minus number of calls to close_child().
      uint32_t m_version_major;		///< Current version (set by set_version(), returned by version()).
      void* m_user_ptr;			///< Current user pointer (set by set_user_ptr(), returned by get_user_ptr());
      /// Constructor.
      state_type(uint32_t version_major, void* user_ptr) : m_depth(0), m_version_major(version_major), m_user_ptr(user_ptr) { }
    };

    state_type  m_state;	/**< \brief Bridge state information.
				  * This state is pushed to m_state_stack prior to processing child
				  * element nodes, but only for nodes with a different name, and
				  * restored after processing them.
				  *
				  * This allows to change this state as part of parsing an element
				  * without influencing the parent element or its children of
				  * a different name.
				  */
    std::stack<state_type> m_state_stack;	///< Internal stack with state information.

  protected:
    /** \brief Construct a Bridge.
      * \param version_major : an initial version to use for this element and its children, if any.
      */
    Bridge(uint32_t version_major) : m_state(version_major, NULL) { }

    /// Virtual destructor.
    virtual ~Bridge() = default;

    /// Push state information onto stack.
    void push_state();
    /// Pop state information from stack.
    void pop_state();

  public:
    /** \brief Accessor for the current version.
      * This version is initially set to the value that was passed to Reader::parse.
      * It can be changed for the current element and its children (but also for
      * siblings with the same name that are yet to be processed) by calling
      * set_version().
      */
    uint32_t version() const { return m_state.m_version_major; }

    /** \brief Set the major version of this particular element.
      * \param version_major : the version to be used by this element and all its children.
      *
      * If not set for a particular element then version() will return the version of the parent element,
      * all the way up to the xml header with a version of equal to the version that was passed
      * to Reader::parse.
      */
    void set_version(uint32_t version_major);

    /// Return true if this bridge is writing to an XML file, false when it is reading.
    virtual bool writing() const = 0;

    /** \brief Specify the name of an element.
      * \param name : the name of the element.
      */
    virtual void node_name(char const* name) = 0;

    /** \brief Read or write a mandatory attribute.
      * \param name : the name of the attribute.
      * \param attribute : a reference to the attribute object.
      *
      * Throws if reading and the attribute does not exist.
      */
    template<typename T>
      void attribute(char const* name, T& attribute);

    /** \brief Read or write an optional attribute.
      * \param name : the name of the attribute.
      * \param attribute : a reference to the attribute object.
      * \param default_value : value to use when reading and attribute is missing.
      * \param always_write : when writing and this is set to false then the attribute is not written when it equals the default value.
      * \returns If the operation was successful.
      */
    template<typename T>
      attribute_return_type optional_attribute(char const* name, T& attribute, T const& default_value = T(), bool always_write = false);

    /** \brief Mandatory attribute with required value.
     * \param name : the name of the attribute.
     * \param value : the required value of this attribute.
     *
     * Throws when reading and \a name is not found or doesn't have the required \a value.
     */
    virtual void attribute(char const* name, char const* value) = 0;

    /** \brief Read or write a child element using xml(Bridge&).
      * \param obj : a reference to the corresponding variable to read from / write to.
      */
    template<typename T>
      void child(T& obj);

    /** \brief Read or write a child element using child_xml(Bridge&).
      * \param obj : a reference to the current parent object which must have a virtual child_xml.
      */
    template<typename T>
      void derived_child(T& obj);

    /** \brief Read or write an optional child element using xml(Bridge&).
      * \param obj : a reference to the corresponding variable to read from / write to.
      * \param default_value : value to use when reading a child element that is missing.
      * \param always_write : when writing and this is set to false then the element is not written when it equals the default value.
      * \returns If the operation was successful.
      */
    template<typename T>
      element_return_type child(T& obj, T const& default_value, bool always_write = true);

    /** \brief Read or write the contents of the current element as text.
      * \param var : a reference to the corresponding variable to read from / write to.
      */
    template<typename T>
      element_return_type text_stream(T& var);

    /** \brief Read or write a child element using read_from_stream / write_to_stream.
      * \param name : the name of the element.
      * \param var : a reference to the corresponding variable to read from / write to.
      * \returns If the operation was successful.
      *
      * This function uses istream/ostream serializers for the type and therefore likely only works for builtin-types!
      * For custom classes use child(T& obj) and have it implement xml(Bridge&).
      */
    template<typename T>
      element_return_type child_stream(char const* name, T& var);

    /** \brief Read or write an optional child element using read_from_stream / write_to_stream.
      * \param name : the name of the element.
      * \param var : a reference to the corresponding variable to read from / write to.
      * \param default_value : value to use when reading a child element that is missing.
      * \param always_write : when writing and this is set to false then the element is not written when it equals the default value.
      * \returns If the operation was successful.
      */
    template<typename T>
      element_return_type child_stream(char const* name, T& var, T const& default_value, bool always_write = true);

    /** \brief Mandatory child element with required text value.
     * \param name : the name of the child.
     * \param value : the required value of the text of this child.
     *
     * Throws when reading and \a name is not found or doesn't have the required \a value.
     */
    virtual void child(char const* name, char const* value) = 0;

    /** \brief Read or write a list of child elements with the same name to or from a std container.
      * \param container : a reference to the corresponding container to read from / write to.
      * \param method : set this to 'xml::insert' when you want use 'insert' to add elements to \a container instead of push_back.
      */
    template<typename CONTAINER, typename METHOD = push_back_method_type>
      void children(CONTAINER& container, METHOD method = push_back);

    /** \brief Read or write a list of child elements with the same \a name to or from a std container.
      * \param name : the name of the elements.
      * \param container : a reference to the corresponding container to read from / write to.
      * \param method : set this to 'xml::insert' when you want use 'insert' to add elements to \a container instead of push_back.
      *
      * This function uses read_from_stream / write_to_stream serializers for value_type.
      * For containers storing custom classes use children(container) and have the child class implement xml(Bridge&),
      * if this is not possible and the default istream/ostream operator is not sufficient, then
      * specialize xml::read_from_stream / xml::write_to_stream for this type.
      */
    template<typename CONTAINER, typename METHOD = push_back_method_type>
      void children_stream(char const* name, CONTAINER& container, METHOD method = push_back);

    /// Set a user pointer.
    void set_user_ptr(void* user_ptr) { m_state.m_user_ptr = user_ptr; }
    /// Get the user pointer that was set with set_user_ptr().
    void* get_user_ptr() const { return m_state.m_user_ptr; }

/// @cond Doxygen_Suppress
  protected:
    // The open_child(T&)/close_child() pair is called around
    // calls to T::xml(Bridge&) when invoking Bridge::child(T&),
    // and the whole series of calls to T::xml(Bridge&) when invoking Bridge::children(CONTAINER&).
    //
    // The open_child(name, T&)/close_child() pair is called around
    // calls to write_child_stream(string)/read_child_stream() when invoking Bridge::child_stream(name, T&).
    //
    // The series open_child_name(name, T&)/[next_child()/.../next_child()/]close_child() are called around
    // calls to write_child_stream(string)/read_child_stream() when invoking Bridge::children_stream(name, T&),
    // although when writing the calls to next_child() are omitted.
    virtual void open_child() = 0;
    virtual void open_child(char const* name) = 0;
    virtual void close_child() = 0;

    // Virtual functions implemented in ReadBridge:
    virtual void get_element();
    virtual void next_child();
    virtual bool read_attribute(char const* name, std::string& attribute_str, bool mandatory) const;
    virtual std::string read_child_stream();
    // Virtual functions only implemented in WriteBridge:
    virtual std::ostream& get_os();
    virtual void write_attribute(char const* name, std::string const& raw_attribute);
    virtual void write_child_stream(std::string const& element);
/// @endcond
};

// Read or write mandatory attribute.
template<typename T>
void Bridge::attribute(char const* name, T& attribute)
{
  if (writing())
  {
    write_attribute(name, write_to_string(attribute));
  }
  else
  {
    std::string attribute_str;
    read_attribute(name, attribute_str, true);
    read_from_string(attribute, attribute_str);
  }
}

// Read or write optional attribute.
template<typename T>
attribute_return_type Bridge::optional_attribute(char const* name, T& attribute, T const& default_value, bool always_write)
{
  if (writing())
  {
    if (always_write || attribute != default_value)
    {
      std::string attribute_str = write_to_string(attribute);
      write_attribute(name, attribute_str);
      return writing_attribute_success;
    }
    return writing_attribute_skipped;
  }
  else
  {
    std::string attribute_str;
    if (read_attribute(name, attribute_str, false))
    {
      read_from_string(attribute, attribute_str);
      return reading_attribute_success;
    }
    else
    {
      Dout(dc::xmlparser, "No attribute '" << name << "'; using \"" << default_value << "\".");
      attribute = default_value;
      return reading_default_attribute;
    }
  }
}

/// @cond Doxygen_Suppress
//  Internal stuff.

// An exception class that is thrown by ReadBridge::state_type::node_name()
// when no child with the specified name is left.
class NoChildLeft : public AIAlert::Error
{
  public:
    NoChildLeft(AIAlert::Prefix const& prefix, AIAlert::modal_nt type, std::string const& xml_desc, AIArgs const& args) :
        AIAlert::Error(prefix, type, xml_desc, args) { }
};

/// @endcond

// Read or write a child element using xml(Bridge&).
template<typename T>
void Bridge::child(T& obj)
{
  open_child();
  obj.xml(*this);
  close_child();
}

// Read or write a child element using child_xml(Bridge&).
template<typename T>
void Bridge::derived_child(T& obj)
{
  open_child();
  obj.child_xml(*this);
  close_child();
}

template<typename T>
element_return_type Bridge::text_stream(T& var)
{
  element_return_type ret;
  if (writing())
  {
    std::ostringstream oss;
    write_to_stream(oss, var);
    write_child_stream(oss.str());
    ret = writing_element_success;
  }
  else
  {
    get_element();
    std::istringstream iss(read_child_stream());
    read_from_stream(iss, var);
    ret = reading_element_success;
  }
  return ret;
}

// Read or write a child element using read_from_stream / write_to_stream.
template<typename T>
element_return_type Bridge::child_stream(char const* name, T& var)
{
  open_child(name);
  element_return_type ret = text_stream(var);
  close_child();
  return ret;
}

// Read or write an optional child element using read_from_stream / write_to_stream.
template<typename T>
element_return_type Bridge::child_stream(char const* name, T& var, T const& default_value, bool always_write)
{
  element_return_type ret = writing_element_skipped;
  int depth = m_state.m_depth;
  try
  {
    if (!writing() || always_write || var != default_value)
    {
      ret = child_stream(name, var);
    }
  }
  catch (NoChildLeft const&)
  {
    close_child();
    if (m_state.m_depth != depth)
      throw;
    var = default_value;
    ret = reading_default_element;
  }
  return ret;
}

// Read or write an optional child element using xml(Bridge&).
template<typename T>
element_return_type Bridge::child(T& obj, T const& default_value, bool always_write)
{
  element_return_type ret = writing_element_skipped;
  open_child();
  int depth = m_state.m_depth;
  try
  {
    if (!writing() || always_write || obj != default_value)
    {
      obj.xml(*this);
      ret = writing() ? writing_element_success : reading_element_success;
    }
  }
  catch (NoChildLeft const&)
  {
    if (m_state.m_depth != depth)
      throw;
    obj = default_value;
    ret = reading_default_element;
  }
  close_child();
  return ret;
}

/// @cond Doxygen_Suppress
//  Internal stuff.

template<typename CONTAINER>
void container_add(CONTAINER& container, typename CONTAINER::value_type const& value, insert_method_type)
{
  container.insert(value);
}

template<typename CONTAINER>
void container_add(CONTAINER& container, typename CONTAINER::value_type const& value, push_back_method_type)
{
  container.push_back(value);
}

/// @endcond

// Read or write a list of child elements with the same name to or from a std container.
template<typename CONTAINER, typename METHOD>
void Bridge::children(CONTAINER& container, METHOD method)
{
  open_child();
  if (writing())
  {
    for (typename CONTAINER::iterator iter = container.begin(); iter != container.end(); ++iter)
    {
      iter->xml(*this);
    }
  }
  else
  {
    int depth = m_state.m_depth;
    for(;;)
    {
      typename CONTAINER::value_type obj;
      try
      {
	obj.xml(*this);
      }
      catch (NoChildLeft const&)
      {
	if (m_state.m_depth != depth)
	  throw;
	break;
      }
      container_add(container, obj, method);
    }
  }
  close_child();
}

// Read or write a list of child elements with the same name to or from a std container using read_from_stream / write_to_stream for the child elements.
template<typename CONTAINER, typename METHOD>
void Bridge::children_stream(char const* name, CONTAINER& container, METHOD method)
{
  if (writing())
  {
    open_child(name);
    for (typename CONTAINER::iterator iter = container.begin(); iter != container.end(); ++iter)
    {
      std::ostringstream oss;
      write_to_stream(oss, *iter);
      write_child_stream(oss.str());
    }
  }
  else
  {
    int depth = m_state.m_depth;
    try
    {
      open_child(name);	// This might throw NoChildLeft, but even then we still need to call close_child().
      for (;;)
      {
	typename CONTAINER::value_type var;
	std::string str = read_child_stream();
	std::istringstream iss(str);
	read_from_stream(iss, var);
	if (iss.fail())
	{
	  THROW_ALERT("Failed to read contents of element <[NAME]> from string \"[STRING]\".",
	      AIArgs("[NAME]", name)("[STRING]", str));
	}
	container_add(container, var, method);
	next_child();
      }
    }
    catch (NoChildLeft const&)
    {
      if (m_state.m_depth != depth + 1)
	throw;
    }
  }
  close_child();
}

} // namespace xml
