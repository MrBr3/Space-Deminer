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

#ifndef _SPACE_DEMINER_BASE_MATH_H_
#define _SPACE_DEMINER_BASE_MATH_H_

#include <glibmm.h>

#include <math.h>

#include "./math/vector2.hpp"
#include "./math/vector3.hpp"

#define PI2 (G_PI*2.0)

/** Gets the integer parte of the binear logarithm of n.
 * */
inline guint blog_int(guint n)
{
  if(n==0)
    return G_MAXUINT;

  guint blog  = 0;

  while(n>1)
  {
    n>>=1;
    ++blog;
  }

  return blog;
}

template<typename T>inline void exchange(T& a, T&b)
{
  T tmp = a;
  a = b;
  b = tmp;
}

inline gfloat get_frac_part(gfloat n){gfloat dummy; return modff(n, &dummy);}
inline gdouble get_frac_part(gdouble n){gdouble dummy; return modf(n, &dummy);}

#endif