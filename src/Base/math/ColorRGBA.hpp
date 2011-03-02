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

  ColorRGBA(const Gdk::Color& c, gfloat a_=1.f)
  {
    r = c.get_red_p();
    g = c.get_green_p();
    b = c.get_blue_p();
    a = a_;
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

  void set(const Gdk::Color& c, gfloat a_=1.f)
  {
    r = c.get_red_p();
    g = c.get_green_p();
    b = c.get_blue_p();
    a = a_;
  }

  void set(float r_, gfloat g_, gfloat b_, gfloat a_=1.f)throw()
  {
    r = r_;
    g = g_;
    b = b_;
    a = a_;
  }

  void set_direction(const Vector3& dir)
  {
    if(fabs(dir.get_square_length()-1.f)>1.e-8)
    {
      Vector3 v=dir;
      v.normalize();
      set(0.5f*v.x+0.5f, 0.5f*v.y+0.5f, 0.5f*v.z+0.5f, 1.f);
      return;
    }

    set(0.5f*dir.x+0.5f, 0.5f*dir.y+0.5f, 0.5f*dir.z+0.5f, 1.f);
  }

  /** \brief Sets the color by a 32 Bit RGBA value
   *
   *
   *
   * \note the caller owns the four bytes and has to make sure they area valid
   * */
  void set(guint8* pixels, int n=4)
  {
    if(!pixels)
      throw std::invalid_argument("**ColorRGBA::set** pixels");
    if(n!=3 && n!=4)
      throw std::invalid_argument("**ColorRGBA::set** n must be 3 or 4");

    static const gfloat f = 1.f/255.f;

    r = pixels[0]*f;
    g = pixels[1]*f;
    b = pixels[2]*f;
    if(n==4)
      a = pixels[3]*f;
    else
      a = 1.f;
  }

  Gdk::Color get_gdk_color()const
  {
    Gdk::Color c;
    c.set_rgb_p(r, g, b);

    return c;
  }

  gfloat get_intensity()const
  {
    return MAX(r, MAX(g, b));
  }

  /** @name  Conversion
   * */
  //@{

  void set_mix_of(const ColorRGBA& a, const ColorRGBA& b, gfloat p1, gfloat p2)
  {
    gfloat p  = p1+p2;

    if(p<=0.f)
    {
      set(0.f, 0.f, 0.f, 0.f);
      return;
    }

    gfloat inv_p  = 1.f/p;

    this->r = (a.r*p1 + b.r*p2) * inv_p;
    this->g = (a.g*p1 + b.g*p2) * inv_p;
    this->b = (a.b*p1 + b.b*p2) * inv_p;
    this->a = (a.a*p1 + b.a*p2) * inv_p;
  }

  /** \brief Mixes two colors in a way, like Gimp would do it using "Normal" Layer Mode.
   *
   * \param above represents the aboves "Layers color"
   * \param below represents the belows "Layers color"
   * \param above_transparency transparency value multiplied with above.a
   * */
  friend ColorRGBA overlay(const ColorRGBA& above, const ColorRGBA& below, gfloat above_transparency=1.f)
  {
    gfloat a  = CLAMP(CLAMP(above_transparency, 0.f, 1.f)*above.a, 0.f, 1.f);
    gfloat om_a = 1.f - a;
    ColorRGBA result;

    result.a = CLAMP(below.a + (below.a+a)*0.5f, 0.f, 1.f);
    if(result.a<=0.f)
    {
      result.r = result.g = result.b = 0.f;
      return result;
    }

    result.r = a*above.r + om_a*below.r;
    result.g = a*above.g + om_a*below.g;
    result.b = a*above.b + om_a*below.b;

    return result;
  }

  static guint8 convert_to_0_255(gfloat x){return round(CLAMP(x*255.f, 0.f, 255.f));}
  static gfloat clamp_to_0_1f(gfloat x){return CLAMP(x, 0.f, 1.f);}

  void clamp()
  {
    r = clamp_to_0_1f(r);
    g = clamp_to_0_1f(g);
    b = clamp_to_0_1f(b);
    a = clamp_to_0_1f(a);
  }

  /** \brief Sets four bytes with the desired color
   *
   * \note the caller owns the four bytes and has to make sure they area valid
   * */
  void fill(guint8* pixels)const
  {
    pixels[0] = convert_to_0_255(r);
    pixels[1] = convert_to_0_255(g);
    pixels[2] = convert_to_0_255(b);
    pixels[3] = convert_to_0_255(a);
  }

  /** \brief Sets four bytes with the desired color and calculates dithering.
   *
   * For Dithering the Floyd–Steinberg algorthm is used. See http://en.wikipedia.org/wiki/Floyd%E2%80%93Steinberg_dithering (2011-01-05) for more information.
   *
   * \note the caller owns the four bytes and has to make sure they area valid
   * */
  void fill_dithered(guint8* pixels, const ColorRGBA& add, ColorRGBA& dither_e, ColorRGBA& dither_se, ColorRGBA& dither_s, ColorRGBA& dither_sw)const
  {
    ColorRGBA resctricted(convert_to_0_255(add.r+r),
                          convert_to_0_255(add.g+g),
                          convert_to_0_255(add.b+b),
                          convert_to_0_255(add.a+a));
    pixels[0] = guint8(resctricted.r);
    pixels[1] = guint8(resctricted.g);
    pixels[2] = guint8(resctricted.b);
    pixels[3] = guint8(resctricted.a);

    ColorRGBA& diff = resctricted;
    diff.r = clamp_to_0_1f(r) - resctricted.r/255.f;
    diff.g = clamp_to_0_1f(g) - resctricted.g/255.f;
    diff.b = clamp_to_0_1f(b) - resctricted.b/255.f;
    diff.a = clamp_to_0_1f(a) - resctricted.a/255.f;

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

  /** @name Multiply
   * */
  //@{
  static ColorRGBA rgb_mult(const ColorRGBA& c1, gfloat f)
  {
    return ColorRGBA(c1.r*f, c1.g*f, c1.g*f, c1.a);
  }

  static ColorRGBA rgb_mult(gfloat f, const ColorRGBA& c1)
  {
    return ColorRGBA(c1.r*f, c1.g*f, c1.g*f, c1.a);
  }
  //@}

  /** @name OpenGL
   * */
  //@{
  void glUniformRGB(GLuint location)const
  {
    glUniform4f(location, r, g, b, 1.f);
  }

  void glUniformRGBA(GLuint location)const
  {
    glUniform4f(location, r, g, b, a);
  }
  //@}
};
#endif
