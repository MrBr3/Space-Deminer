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

   /** \brief Does nothing (use it only for performance reasons)
    *
    * The values of <tt>x</tt>, <tt>y</tt>, <tt>z</tt> are unkown.
    *
    * \param  di pass <tt>DONT_INIT</tt>
    * */
  Vector3(DontInit di)throw()
  {
  }

  /** \brief Constructor setting all components to 0
   * */
  Vector3()throw()
  {
    x = 0.f;
    y = 0.f;
    z = 0.f;
  }

  Vector3(const Vector2& other)throw()
  {
    x = other.x;
    y = other.y;
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

  void set(const Vector2& other)throw()
  {
    x = other.x;
    y = other.y;
    z = 0.f;
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

  /** \brief Sets vector to represent the cross product with another vector   *
   * */
  Vector3& set_cross(const Vector3& v)throw()
  {
    gfloat _x  = x;
    gfloat _y  = y;

    x  =  y*v.z -  z*v.y;
    y  =  z*v.x - _x*v.z;
    z  = _x*v.y - _y*v.x;

    return *this;
  }

  /** \brief Calcs a vector representing the cross product with another vector
   * */
   Vector3 cross(const Vector3& b)const throw()
   {
     Vector3 v  = *this;
     v.set_cross(b);
     return v;
   }

  /** \brief Calcs a vector representing the cross product with another vector
   * */
  friend Vector3 cross(const Vector3& a, const Vector3& b)throw()
  {
    return a.cross(b);
  }
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
    if(fabs(get_square_length()-1.f)>1.e-7 && (x!=0.f || y!=0.f || z!=0.f))
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

  /** @name Debugging
   * */
  //@{
  std::string str()const
  {
    return Glib::ustring::compose("(%1, %2, %3)", x, y, z).c_str();
  }
  //@}
};

#endif