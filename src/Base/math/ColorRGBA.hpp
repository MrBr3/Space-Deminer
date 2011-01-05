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

  void set(gfloat grey=0.f, gfloat a_=1.f)throw()
  {
    r = grey;
    g = grey;
    b = grey;
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

  /** \brief Sets four bytes with the desired color and calculates dithering.
   *
   * For Dithering the Floyd–Steinberg algorthm is used. See http://en.wikipedia.org/wiki/Floyd%E2%80%93Steinberg_dithering (2011-01-05) for more information.
   *
   * \note the caller owns the four bytes and has to make sure they area valid
   * */
  void fill_dithered(guint8* pixels, const ColorRGBA& add, ColorRGBA& dither_e, ColorRGBA& dither_se, ColorRGBA& dither_s, ColorRGBA& dither_sw)
  {
    ColorRGBA resctricted(CLAMP(round((add.r+r)*255.f), 0, 255),
                          CLAMP(round((add.g+g)*255.f), 0, 255),
                          CLAMP(round((add.b+b)*255.f), 0, 255),
                          CLAMP(round((add.a+a)*255.f), 0, 255));
    pixels[0] = guint8(resctricted.r);
    pixels[1] = guint8(resctricted.g);
    pixels[2] = guint8(resctricted.b);
    pixels[3] = guint8(resctricted.a);

    ColorRGBA& diff = resctricted;
    diff.r = r - resctricted.r/255.f;
    diff.g = g - resctricted.g/255.f;
    diff.b = b - resctricted.b/255.f;
    diff.a = a - resctricted.a/255.f;

    const gfloat _7  = 7.f/16.f;
    const gfloat _1  = 1.f/16.f;
    const gfloat _5  = 5.f/16.f;
    const gfloat _3  = 3.f/16.f;
    dither_e.r  += diff.r * _7;
    dither_e.g  += diff.g * _7;
    dither_e.b  += diff.b * _7;
    dither_e.a  += diff.a * _7;

    dither_se.r = diff.r * _1;
    dither_se.g = diff.g * _1;
    dither_se.b = diff.b * _1;
    dither_se.a = diff.a * _1;

    dither_s.r  = diff.r * _5;
    dither_s.g  = diff.g * _5;
    dither_s.b  = diff.b * _5;
    dither_s.a  = diff.a * _5;

    dither_sw.r += diff.r * _3;
    dither_sw.g += diff.g * _3;
    dither_sw.b += diff.b * _3;
    dither_sw.a += diff.a * _3;
  }
  //@}
};
#endif