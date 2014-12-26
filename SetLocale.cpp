/**
 * \file SetLocale.cpp
 * \brief This file contains the implementation of class SetLocale.
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

#include "SetLocale.h"

namespace xml {

void SetLocale::set_input_locale(std::istream& UNUSED_ARG(is)) const
{
}

void SetLocale::set_output_locale(std::ostream& UNUSED_ARG(os)) const
{
}

} // namespace xml
