/* Space Deminer is a small 2D Arcade Game. Your task ist to eliminate
 * intelligent Mines created in the last galactic war.
 *
 * Copyright (C) 2010 the Space Deminer Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SPACE_DEMINER_BASE_TEMPLATES_H_
#define _SPACE_DEMINER_BASE_TEMPLATES_H_

/** \brief A Marshaller for Sigc Signals
*
* \param def_value - if the return Value is not equal to this value, no other signal slots are executed
*/
template<bool def_value>
class InterruptableSigcAccumulator
{
public:
  typedef bool result_type;

  template<typename T_iterator>
  result_type operator()(T_iterator first, T_iterator last)const
  {
    for(; first!=last; ++first)
    {
      if(*first!=def_value)
        return !def_value;
    }

    return def_value;
  }
};

#endif