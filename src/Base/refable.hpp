/* Space Deminer is a small 2D Arcade Game. Your task ist to eliminate
 * intelligent Mines created in the past galactic war.
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

#ifndef _SPACE_DEMINER_BASE_REFABLE_H_
#define _SPACE_DEMINER_BASE_REFABLE_H_

#include <sigc++/trackable.h>

#include <glibmm.h>

#include "./dependencies.hpp"

/** \brief Refable is class, which provides a reference an a unreference function in order to be managed by a Glib::RefPtr.
*
* In order to use this Function, just create a inherited class.
*/
class Refable : public sigc::trackable
{
  mutable gsize _ref_count;

public:
  /**\brief Gets the current value of the reference counter.
  */
  gsize get_ref_count()const{return _ref_count;}

public:
  /**\brief Increments the internal reference counter.
  *
  *  In normal case this Function is called automatically by RefPtr - so you shouldn't be needed to call this manually.
  *
  *\sa unreference
  */
  void reference()const
  {
    _ref_count++;
  }

  /**\brief Decrements the internal reference counter. And deletes the Object, if zero is reached
  *
  * In normal case this Function is called automatically by RefPtr - so you shouldn't be needed to call this manually.
  *
  *\sa reference
  */
  void unreference()const
  {
    _ref_count--;
    if(_ref_count==0)
      delete this;
    g_assert(_ref_count>=0);
  }

public:
  /** \brief Makes sure, the internal reference counter won't be copied, too.
  *
  * The reference counter is not changed afterwards.
  *
  * \see Refable::Refable
  */
  Refable& operator=(const Refable& r){return *this;}

  /** \brief Makes sure, the internal reference counter won't be copied, too.
  *
  * The reference counter is initialized with 1 (one).
  *
  * \test
  *
  * \see Refable::Refable
  */
  Refable(const Refable& r)
  {
    _ref_count=1;
  }

public:
  /** \brief Initializes the reference counter with 1 (one).
  *
  * This neccessary, as a RefPtr doesn't call Refable::reference, when getting it assigned.
  */
  Refable()
  {
    _ref_count=1;
  }
  virtual ~Refable()throw(){}
};

#endif