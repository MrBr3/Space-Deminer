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

#ifndef _SPACE_DEMINER_BASE_MATH_VECTOR3_H_
#define _SPACE_DEMINER_BASE_MATH_VECTOR3_H_

class Vector3
{
public:
  gfloat x, y, z;

  Vector3(gfloat x_, gfloat y_, gfloat z_)throw()
  {
    x = x_;
    y = y_;
    z = z_;
  }

  /** \brief Constructor setting all components to 0
   * */
  Vector3()throw()
  {
    x = 0.f;
    y = 0.f;
    z = 0.f;
  }

  Vector3(const Vector3& other)throw()
  {
    x = other.x;
    y = other.y;
    z = other.z;
  }

  void set(gfloat all)throw()
  {
    x=y=z=all;
  }

  void set(gfloat x_, gfloat y_, gfloat z_)throw()
  {
    x = x_;
    y = y_;
    z = z_;
  }

  void set(const Vector3& other)throw()
  {
    x = other.x;
    y = other.y;
    z = other.z;
  }

  /** @name Add & Subtract
   * */
  //@{
  Vector3 operator+(const Vector3& v)const throw()
  {
    return Vector3(x+v.x, y+v.y, z+v.z);
  }

  Vector3& operator+=(const Vector3& v)throw()
  {
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
  }
  Vector3 operator-(const Vector3& v)const throw()
  {
    return Vector3(x-v.x, y-v.y, z-v.z);
  }

  Vector3& operator-=(const Vector3& v)throw()
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return *this;
  }
  //@}

  /** @name Products & Quotients
   * */
  //@{
  Vector3 operator*(gfloat a)const throw(){return Vector3(x*a, y*a, z*a);}

  /** \brief Multiplies each component of the Vector with a.
   *
   * \param a a real number
   *
   * \return A reference to the Vector
   * */
  Vector3& operator*=(gfloat a) throw()
  {
   x  *= a;
   y  *= a;
   z  *= a;
   return *this;
  }

  /** \brief Calcs the Scalar product of this and the second vector
   * */
  gfloat operator*(const Vector3& b)const throw(){return x*b.x + y*b.y + z*b.z;}
  //@}

  /** @name Lengths
   * */
  //@{
  /** \brief Gets the length of the vector squared.
   *
   * \return The length of the vector squared
   * */
  gfloat get_square_length()const throw()
  {
    return x*x + y*y + z*z;
  }

  /** \brief Gets the length of the vector.
   *
   * \return The length of the vector
   * */
  gfloat get_length()const throw()
  {
    return sqrt(get_square_length());
  }

  /** \brief Sets the Length of the Vector to 1.
   *
   * A vector without length won't be touched.
   * */
  void normalize()throw()
  {
    if(x!=0.f || y!=0.f || z!=0.f)
    {
      gfloat tmp  = get_length();
      g_assert(tmp!=0.f);

      tmp = 1.f/tmp;

      x *= tmp;
      y *= tmp;
      z *= tmp;
    }
  }
  //@}
};

#endif