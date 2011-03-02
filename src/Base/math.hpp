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

#ifndef _SPACE_DEMINER_BASE_MATH_H_
#define _SPACE_DEMINER_BASE_MATH_H_

#include <glibmm.h>

#include <math.h>

#include "./dependencies.hpp"

#define PI2 (G_PI*2.0)
#define INV_SQRT_2 0.7071067811865475f
#define INV_PI 0.318309886183790672f
#define INV_PI2 0.636619772367581343f

const gfloat degree = G_PI/180.f;

template<class T>
inline T abs(const T& t)
{
  if(t<T(0))
    return -t;
  else
    return t;
}

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

inline gfloat square(gfloat x){return x*x;}
inline gdouble square(gdouble x){return x*x;}

/** \brief Solves a quadric formula.
 *
 * This functions solves the equation \f$0=ax^2+bx+c\f$, if the Variables \f$a\f$, \f$b\f$ and \f$c\f$ are known.
 *
 * \param x_1 the first solution of the equation. If the equation is not solvable, this value won't be changed
 * \param x_2 the first solution of the equation. If the equation is not solvable, this value won't be changed
 * \param a
 * \param b
 * \param c
 *
 * \return false if the equation is not solvable.
 * */
inline bool solve_quadric_formula(gfloat& x_1, gfloat& x_2, gfloat a, gfloat b, gfloat c)throw(std::invalid_argument)
{
  if(a==0)
    throw std::invalid_argument("**solve_quadric_formula** a is not allowed to be==0\n");

  gfloat d  = b*b-4.f*a*c;

  if(d<0)
    return false;

  d = sqrt(d);
  gfloat ia2  = 1.f/(2.f*a);

  x_1 = (-b+d)*ia2;
  x_2 = (-b-d)*ia2;
  return true;
}

/** \brief Gets the angle described by x and y
 *
 * The vector described by x and y must be normalized.
 *
 * The following example will return a:
 * \code
 * get_angle_from_dir(cos(a*PI), sin(a*PI));
 * \endcode
 *
 * \return The angle In the Interval [0, 1] (1 stands for 360°)
 * */
inline gfloat get_angle_from_dir(gfloat x, gfloat y)
{
  gfloat a  = acos(CLAMP(x, -1.f, 1.f))/PI2;
  if(y<0.f)
    a = 1.f - a;
  return a;
}

inline gfloat get_frac_part(gfloat n){gfloat dummy; return modff(n, &dummy);}
inline gdouble get_frac_part(gdouble n){gdouble dummy; return modf(n, &dummy);}

inline bool XOR(bool a, bool b){return a!=b;}

namespace Private
{
  const gdouble inv_rand_max = 1.f/gfloat(RAND_MAX);
}

/** \brief returns a random real number within the interval [0, 1]
 * */
inline gfloat frand()
{
  return CLAMP(gdouble(rand())*Private::inv_rand_max, 0.f, 1.f);
}

namespace Math
{
  enum Overlap
  {
    OVERLAPPING, BACKSIDE, FORESIDE, INSIDE, OUTSIDE
  };

  class Ray;
  class Sphere;

#include "./math/Vector2.hpp"
#include "./math/Vector3.hpp"
#include "./math/Vector4.hpp"
#include "./math/Matrix44.hpp"
#include "./math/ColorRGBA.hpp"
#include "./math/Ray.hpp"
#include "./math/Plane.hpp"
#include "./math/Geometry.hpp"
#include "./math/Sphere.hpp"
}

using Math::Vector2;
using Math::Vector3;
using Math::Vector4;
using Math::Matrix44;
using Math::Plane;
using Math::ColorRGBA;
#endif
