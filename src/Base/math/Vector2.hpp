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

   /** \brief Does nothing (use it only for performance reasons)
    *
    * The values of <tt>x</tt>, <tt>y</tt> are unkown.
    *
    * \param  di pass <tt>DONT_INIT</tt>
    * */
  Vector2(DontInit di)throw()
  {
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

  /**@name Sums
   * */
  //@{
  Vector2 operator-(const Vector2& v)const throw(){return Vector2(x*-v.x, y-v.y);}

  /**@name Products & Quotients
   * */
  //@{
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

  /** \brief Gets the scalar product.
   * */
  gfloat operator*(const Vector2& v)const throw(){return v.x*x + v.y*y;}
  //@}

  /**@name Mapping
   * */
  //@{
  /** \brief Rotates the Vector clock-wise about 90 degrees
   * */
  Vector2& rotate_90_cw()throw()
  {
    gfloat _x_  = x;
    x =  y;
    y =-_x_;

   return *this;
  }

  /** \brief Rotates the Vector counter-clock-wise about 90 degrees
   * */
  Vector2& rotate_90_ccw()throw()
  {
    gfloat _x_  = x;
    x = -y;
    y = _x_;

   return *this;
  }
  //@}

  /**@name Add & Subtract
   * */
  //@{
  Vector2 operator+(const Vector2& v)const throw()
  {
    return Vector2(x+v.x, y+v.y);
  }

  Vector2& operator+=(const Vector2& v)throw()
  {
    x += v.x;
    y += v.y;

    return *this;
  }
  //@}

  /**@name Length
   * */
  //@{
  /** \brief Gets the length of the vector squared.
   *
   * \return The length of the vector squared
   * */
  gfloat get_square_length()const throw()
  {
    return x*x + y*y;
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
  void normalize() throw()
  {
    if(x!=0.f || y!=0.f)
    {
      gfloat tmp  = get_length();
      g_assert(tmp!=0.f);

      tmp = 1.f/tmp;

      x *= tmp;
      y *= tmp;
    }
  }
  //@}

  /** @name Debugging
   * */
  //@{
  std::string str()const
  {
    return Glib::ustring::compose("(%1, %2)", x, y).c_str();
  }
  //@}
};

#endif