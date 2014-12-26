/**
 * \file catalog_test.cpp
 * \brief Example code that shows how to use ai-xml.
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

#include "Reader.h"
#include "Writer.h"

#ifndef USE_PCH
#include "debug.h"
#include <string>
#include <map>
#include <vector>
#include <list>
#include <boost/filesystem.hpp>
#endif

//#define PRINT_DEBUG

namespace fs = boost::filesystem;

class ColorSwatch
{
  private:
    std::string m_color;
  public:
    void xml(xml::Bridge& xml);
};

class Size
{
  private:
    std::string m_description;
    std::vector<ColorSwatch> m_color_swatches;
  public:
    void xml(xml::Bridge& xml);
};

class CatalogItem
{
  private:
    std::string m_gender;
    std::string m_item_number;
    float m_price;
    std::vector<Size> m_sizes;
  public:
    void xml(xml::Bridge& xml);
};

class Product
{
  public:
    typedef std::map<std::string, std::string> image_map_type;
  private:
    std::string m_description;
    std::string m_product_image;
    image_map_type m_image_map;
    std::vector<CatalogItem> m_items;
  public:
    image_map_type& map(void) { return m_image_map; }
    void xml(xml::Bridge& xml);
};

class Catalog
{
  private:
    std::list<Product> m_products;
  public:
    void xml(xml::Bridge& xml);
};

void Catalog::xml(xml::Bridge& xml)
{
  xml.node_name("catalog");
  xml.children(m_products);
}

void Product::xml(xml::Bridge& xml)
{
  xml.node_name("product");
  xml.attribute("description", m_description);
  xml.attribute("product_image", m_product_image);
  xml.set_user_ptr(this);
  xml.children(m_items);
  Dout(dc::notice, "Read " << m_items.size() << " items and " << m_image_map.size() << " name->image mappings:");
  for (auto iter = m_image_map.begin(); iter != m_image_map.end(); ++iter)
  {
    Dout(dc::notice, "  " << iter->first << " --> " << iter->second);
  }
}

void CatalogItem::xml(xml::Bridge& xml)
{
  xml.node_name("catalog_item");
  xml.attribute("gender", m_gender);
  xml.child_stream("item_number", m_item_number);
  xml.child_stream("price", m_price);
  xml.children(m_sizes);
}

void Size::xml(xml::Bridge& xml)
{
  xml.node_name("size");
  xml.attribute("description", m_description);
  xml.children(m_color_swatches);
}

void ColorSwatch::xml(xml::Bridge& xml)
{
  Product* product = static_cast<Product*>(xml.get_user_ptr());

  xml.node_name("color_swatch");
  std::string* name;
  std::string* image;
  if (xml.writing())
  {
    name = &m_color;
    image = &product->map()[m_color];
  }
  else
  {
    name = new std::string;
    image = new std::string;
  }
  xml.attribute("image", *image);
  xml.text_stream(*name);
  if (!xml.writing())
  {
    m_color = *name;
    product->map().insert(Product::image_map_type::value_type(*name, *image));
    delete image;
    delete name;
  }
}

int main(int argc, char* argv[])
{
  Debug(debug::init());

  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " catalog_test.xml" << std::endl;
    return 1;
  }

  fs::path filepath(argv[1]);
  xml::Reader reader;

  Catalog catalog;
  try
  {
    reader.parse(filepath, 1);
    catalog.xml(reader);
  }
  catch(AIAlert::Error const& error)
  {
    //std::cerr << AIAlert::Error(AIAlert::Prefix(), AIAlert::not_modal, "Failed to load [FILE]: ",
    //	AIArgs("[FILE]", filepath.native()), error) << std::endl;
    return 1;
  }
  catch(std::runtime_error const& error)
  {
    std::cerr << "Failed to load " << filepath << ": " << error.what() << std::endl;
    return 1;
  }

#ifdef PRINT_DEBUG
  DebugBuf buf;
  std::ostream dout(&buf);
  xml::Writer writer(dout);
#else
  Debug(libcw_do.off());
  xml::Writer writer(std::cout);
#endif

  writer.write(catalog);

#ifndef PRINT_DEBUG
  Debug(libcw_do.on());
#endif
}
