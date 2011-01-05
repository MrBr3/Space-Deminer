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

#ifndef _SPACE_DEMINER_BASE_MATH_COLOR_RGBA_H_
#define _SPACE_DEMINER_BASE_MATH_COLOR_RGBA_H_

class ColorRGBA
{
public:
  gfloat r, g, b, a;

  /** \brief Does nothing (use it only for performance reasons)
  *
  * The values of <tt>r/tt>, <tt>g</tt>, <tt>b</tt>, <tt>a</tt> are unkown.
  *
  * \param  di pass <tt>DONT_INIT</tt>
  * */
  ColorRGBA(DontInit di)throw()
  {
  }

  ColorRGBA(gfloat grey=0.f, gfloat a_=1.f)throw()
  {
    r = grey;
    g = grey;
    b = grey;
    a = a_;
  }

  ColorRGBA(gfloat r_, gfloat g_, gfloat b_, gfloat a_=1.f)throw()
  {
    r = r_;
    g = g_;
    b = b_;
    a = a_;
  }

  void set(float r_, gfloat g_, gfloat b_, gfloat a_=1.f)throw()
  {
    r = r_;
    g = g_;
    b = b_;
    a = a_;
  }

  /** @name  Conversion
   * */
  //@{

  /** \brief Sets four bytes with the desired color
   *
   * \note the caller owns the four bytes and has to make sure they area valid
   * */
  void fill(guint8* pixels)
  {
    pixels[0] = CLAMP(guint8(r*255.f), 0, 255);
    pixels[1] = CLAMP(guint8(g*255.f), 0, 255);
    pixels[2] = CLAMP(guint8(b*255.f), 0, 255);
    pixels[3] = CLAMP(guint8(a*255.f), 0, 255);
  }
  //@}
};
#endif