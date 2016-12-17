/**
 * \file set_locale_for.h
 * \brief This file contains the declaration of template function set_locale_for.
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
 * \class xml::set_locale_for
 * \brief Helper template class to specialize for SetLocale derived classes.
 */

#include "SetLocale.h"

#ifndef USE_PCH
#include <type_traits>
#endif

#ifndef XML_SET_LOCALE_FOR_H
#define XML_SET_LOCALE_FOR_H

namespace xml {

/// Provides a hook to set a locale (facet) on an istream/ostream. The default does nothing.
template<typename T, bool = std::is_base_of<SetLocale, T>::value>
struct set_locale_for {
  /// Stub.
  static void for_stream(std::istream& UNUSED_ARG(is), T const& UNUSED_ARG(var)) { }
  /// Stub.
  static void for_stream(std::ostream& UNUSED_ARG(is), T const& UNUSED_ARG(var)) { }
};

/// Specialization that provides a hook to set a locale (facet) on an istream/ostream for classes derived from SetLocale.
template<typename T>
struct set_locale_for<T, true> {
  /// Call SetLocale::set_input_locale.
  static void for_stream(std::istream& is, T const& var) { var.set_input_locale(is); }
  /// Call SetLocale::set_output_locale.
  static void for_stream(std::ostream& os, T const& var) { var.set_output_locale(os); }
};

} // namespace xml

#endif // XML_SET_LOCALE_FOR_H
