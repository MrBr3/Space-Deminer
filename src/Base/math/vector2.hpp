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

#ifndef _SPACE_DEMINER_BASE_MATH_VECTOR2_H_
#define _SPACE_DEMINER_BASE_MATH_VECTOR2_H_

class Vector2
{
public:
  gfloat x, y;

  Vector2(gfloat x_, gfloat y_) throw()
  {
    x = x_;
    y = y_;
  }

  /** \brief Constructor setting all components to 0
   * */
  Vector2() throw()
  {
    x = 0.f;
    y = 0.f;
  }

  Vector2(const Vector2& other) throw()
  {
    x = other.x;
    y = other.y;
  }

  void set(gfloat all) throw()
  {
    x=y=all;
  }

  void set(gfloat x_, gfloat y_) throw()
  {
    x = x_;
    y = y_;
  }

  void set(const Vector2& other) throw()
  {
    x = other.x;
    y = other.y;
  }

  /** \brief Multiplies each component of the Vector with a.
   *
   * \param a a real number
   *
   * \return A reference to the Vector
   * */
   Vector2& operator*=(gfloat a) throw()
   {
     x  *= a;
     y  *= a;
     return *this;
   }

  Vector2 operator*(gfloat a)const throw(){return Vector2(x*a, y*a);}

  /** \brief Gets the length of the vector squared.
   *
   * \return The length of the vector squared
   * */
  gfloat square_length()const throw()
  {
    return x*x + y*y;
  }

  /** \brief Gets the length of the vector.
   *
   * \return The length of the vector
   * */
  gfloat length()const throw()
  {
    return sqrt(square_length());
  }

  /** \brief Sets the Length of the Vector to 1.
   *
   * A vector without length won't be touched.
   * */
  void normalize() throw()
  {
    if(x!=0.f || y!=0.f)
    {
      gfloat tmp  = length();
      g_assert(tmp!=0.f);

      tmp = 1.f/tmp;

      x *= tmp;
      y *= tmp;
    }
  }
};

#endif