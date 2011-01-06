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

#ifndef _SPACE_DEMINER_BASE_MATH_VECTOR4_H_
#define _SPACE_DEMINER_BASE_MATH_VECTOR4_H_

class Vector4
{
public:
  gfloat x, y, z, w;

  Vector4(gfloat x_, gfloat y_, gfloat z_, gfloat w_=1.f)throw()
  {
    x = x_;
    y = y_;
    z = z_;
    w = w_;
  }

   /** \brief Does nothing (use it only for performance reasons)
    *
    * The values of <tt>x</tt>, <tt>y</tt>, <tt>z</tt> are unkown.
    *
    * \param  di pass <tt>DONT_INIT</tt>
    * */
  Vector4(DontInit di)throw()
  {
  }

  /** \brief Constructor setting all components to 0
   * */
  Vector4()throw()
  {
    x = 0.f;
    y = 0.f;
    z = 0.f;
    w = 1.f;
  }

  Vector4(const Vector2& other)throw()
  {
    x = other.x;
    y = other.y;
    z = 0.f;
    w = 1.f;
  }

  Vector4(const Vector3& other)throw()
  {
    x = other.x;
    y = other.y;
    z = other.z;
    w = 1.f;
  }

  Vector4(const Vector4& other)throw()
  {
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
  }

  void set(gfloat all gfloat w_=1.f)throw()
  {
    x=y=z=all;
    w = w_;
  }

  void set(gfloat x_, gfloat y_, gfloat z_, gfloat w_)throw()
  {
    x = x_;
    y = y_;
    z = z_;
    w = w_;
  }

  void set(const Vector2& other)throw()
  {
    x = other.x;
    y = other.y;
    z = 0.f;
    w = 1.f;
  }

  void set(const Vector3& other)throw()
  {
    x = other.x;
    y = other.y;
    z = other.z;
    w = 1.f;
  }

  void set(const Vector4& other)throw()
  {
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
  }

  operator Vector3()const
  {
    if(w==0.f)
    {
      g_warning("**Vector4::operator Vector3** w==0");
      return Vector3(x, y, z);
    }
    gfloat inv_w  = 1.f/w;
    return Vector3(x*inv_w, y*inv_w, z*inv_w);
  }

  /* TODO make to doxygen comment* @name Add & Subtract
   * */
  //@ TODO make to doxygen comment {
  /*Vector4 operator+(const Vector4& v)const throw()
  {
    return Vector4(x+v.x, y+v.y, z+v.z, TODO decide what to do here);
  }

  Vector4& operator+=(const Vector4& v)throw()
  {
    x += v.x;
    y += v.y;
    z += v.z;
    w TODO decide what todo here

    return *this;
  }
  Vector4 operator-(const Vector4& v)const throw()
  {
    return Vector4(x-v.x, y-v.y, z-v.z, TODO decide what to do here);
  }

  Vector4& operator-=(const Vector4& v)throw()
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w TODO decide what todo here

    return *this;
  }*/
  //@ TODO make to doxygen comment }

  /*TODO make to doxygen comment* @name Products & Quotients
   * */
  //@ TODO make to doxygen comment{
  //Vector4 operator*(gfloat a)const throw(){return Vector4(x*a, y*a, z*a, TODO decide what to do here);}

  /** \brief Multiplies each component of the Vector with a.
   *
   * \param a a real number
   *
   * \return A reference to the Vector
   * */
  /*Vector4& operator*=(gfloat a) throw()
  {
   x  *= a;
   y  *= a;
   z  *= a;
    w TODO decide what todo here
   return *this;
  }*/

  /** \brief Calcs the Scalar product of this and the second vector
   * */
  gfloat operator*(const Vector4& b)const throw(){return x*b.x + y*b.y + z*b.z + w*b.w;}
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
    return x*x + y*y + z*z + w*w; // TODO check,whether w should also be usedfor w
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
      w *= tmp; // TODO check,whether w should also be usedfor w
    }
  }
  //@}

  /** @name Debugging
   * */
  //@{
  operator const char*()const
  {
    return Glib::ustring::compose("(%1, %2, %3, %4)", x, y, z, w).c_str();
  }
  //@}
};

#endif