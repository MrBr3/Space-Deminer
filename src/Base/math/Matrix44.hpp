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

#ifndef _SPACE_DEMINER_BASE_MATH_MATRIX44_H_
#define _SPACE_DEMINER_BASE_MATH_MATRIX44_H_

class Matrix44
{
public:
  gfloat m[16];

  Matrix44()throw()
  {
    set_identity();
  }

  Matrix44(gfloat m11, gfloat m12, gfloat m13, gfloat m14,
           gfloat m21, gfloat m22, gfloat m23, gfloat m24,
           gfloat m31, gfloat m32, gfloat m33, gfloat m34,
           gfloat m41, gfloat m42, gfloat m43, gfloat m44)throw()
  {
    m[0]   = m11;
    m[1]   = m21;
    m[2]   = m31;
    m[3]   = m41;

    m[4]   = m12;
    m[5]   = m22;
    m[6]   = m32;
    m[7]   = m42;

    m[8]   = m13;
    m[9]   = m23;
    m[10]  = m33;
    m[11]  = m43;

    m[12]  = m14;
    m[13]  = m24;
    m[14]  = m34;
    m[15]  = m44;
  }

  Matrix44(DontInit di)throw()
  {
  }

  void set_identity()throw()
  {
    set(1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f);
  }

  void set(gfloat m11, gfloat m12, gfloat m13, gfloat m14,
           gfloat m21, gfloat m22, gfloat m23, gfloat m24,
           gfloat m31, gfloat m32, gfloat m33, gfloat m34,
           gfloat m41, gfloat m42, gfloat m43, gfloat m44)throw()
  {
    m[0]   = m11;
    m[1]   = m21;
    m[2]   = m31;
    m[3]   = m41;

    m[4]   = m12;
    m[5]   = m22;
    m[6]   = m32;
    m[7]   = m42;

    m[8]   = m13;
    m[9]   = m23;
    m[10]  = m33;
    m[11]  = m43;

    m[12]  = m14;
    m[13]  = m24;
    m[14]  = m34;
    m[15]  = m44;
  }
};

#endif