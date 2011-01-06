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

  Matrix44(const Vector4& column1, const Vector4& column2, const Vector4& column3, const Vector4& column4)throw()
  {
    set_by_columns(column1, column2, column3, column4);
  }

  Matrix44(const Vector4& column1, const Vector4& column2, const Vector4& column3, const Vector3& column4, gfloat m44=1.f)throw()
  {
    set_by_columns(column1, column2, column3, column4, m44);
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

  void set_by_columns(const Vector4& column1, const Vector4& column2, const Vector4& column3, const Vector4& column4)throw()
  {
    m[0]  = column1.x;
    m[1]  = column1.y;
    m[2]  = column1.z;
    m[3]  = column1.w;

    m[4]  = column2.x;
    m[5]  = column2.y;
    m[6]  = column2.z;
    m[7]  = column2.w;

    m[8]  = column3.x;
    m[9]  = column3.y;
    m[10] = column3.z;
    m[11] = column3.w;

    m[12] = column4.x;
    m[13] = column4.y;
    m[14] = column4.z;
    m[15] = column4.w;
  }

  void set_by_columns(const Vector4& column1, const Vector4& column2, const Vector4& column3, const Vector3& column4, gfloat m44=1.f)throw()
  {
    m[0]  = column1.x;
    m[1]  = column1.y;
    m[2]  = column1.z;
    m[3]  = column1.w;

    m[4]  = column2.x;
    m[5]  = column2.y;
    m[6]  = column2.z;
    m[7]  = column2.w;

    m[8]  = column3.x;
    m[9]  = column3.y;
    m[10] = column3.z;
    m[11] = column3.w;

    m[12] = column4.x;
    m[13] = column4.y;
    m[14] = column4.z;
    m[15] = m44;
  }

  void set_by_rows(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4)throw()
  {
    m[0]  = row1.x;
    m[1]  = row2.x;
    m[2]  = row3.x;
    m[3]  = row4.x;

    m[4]  = row1.y;
    m[5]  = row2.y;
    m[6]  = row3.y;
    m[7]  = row4.y;

    m[8]  = row1.z;
    m[9]  = row2.z;
    m[10] = row3.z;
    m[11] = row4.z;

    m[12] = row1.w;
    m[13] = row2.w;
    m[14] = row3.w;
    m[15] = row4.w;
  }

  void set_by_rows(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector3& row4, gfloat m44=1.f)throw()
  {
    m[0]  = row1.x;
    m[1]  = row2.x;
    m[2]  = row3.x;
    m[3]  = row4.x;

    m[4]  = row1.y;
    m[5]  = row2.y;
    m[6]  = row3.y;
    m[7]  = row4.y;

    m[8]  = row1.z;
    m[9]  = row2.z;
    m[10] = row3.z;
    m[11] = row4.z;

    m[12] = row1.w;
    m[13] = row2.w;
    m[14] = row3.w;
    m[15] = m44;
  }

  void set_column(gsize c, const Vector4& column)throw(std::invalid_argument)
  {
    if(c<1 || c>4)
      throw std::invalid_argument("**Matrix44::set_column** c must be one of {1, 2, 3, 4}");

    c = (c-1)*4;

    m[0+c]  = column.x;
    m[1+c]  = column.y;
    m[2+c]  = column.z;
    m[3+c]  = column.w;
  }

  void set_row(gsize r, const Vector4& column)throw(std::invalid_argument)
  {
    if(r<1 || r>4)
      throw std::invalid_argument("**Matrix44::set_row** r must be one of {1, 2, 3, 4}");

    r = (r-1);

    m[0+r]  = column.x;
    m[4+r]  = column.y;
    m[8+r]  = column.z;
    m[12+r]  = column.w;
  }

  const gfloat& operator()(gsize r, gsize c)const throw(std::invalid_argument)
  {
    if(c<1 || c>4)
      throw std::invalid_argument("**Matrix44::operator()** c must be one of {1, 2, 3, 4}");
    if(r<1 || r>4)
      throw std::invalid_argument("**Matrix44::operator()** r must be one of {1, 2, 3, 4}");

    return m[(c-1)*4+r-1];
  }

  gfloat& operator()(gsize r, gsize c)throw(std::invalid_argument)
  {
    if(c<1 || c>4)
      throw std::invalid_argument("**Matrix44::operator()** c must be one of {1, 2, 3, 4}");
    if(r<1 || r>4)
      throw std::invalid_argument("**Matrix44::operator()** r must be one of {1, 2, 3, 4}");

    return m[(c-1)*4+r-1];
  }

  /** \brief Gets a single column
   *
   * \param i the column to use (must be one of {1, 2, 3, 4} otherwise a std::invalid_argument is thrown)
   * */
  Vector4 get_column(gsize i)const throw(std::invalid_argument)
  {
    switch(i)
    {
    case 1:
      return Vector4(m[0],  m[1],  m[2],  m[3]);
    case 2:
      return Vector4(m[4],  m[5],  m[6],  m[7]);
    case 3:
      return Vector4(m[8],  m[9],  m[10], m[11]);
    case 4:
      return Vector4(m[12], m[13], m[14], m[15]);
    default:
      throw std::invalid_argument("**Matrix44::get_column** i must be one of {1, 2, 3, 4}");
    }
  }

  /** \brief Gets a single row
   *
   * \param i the column to use (must be one of {1, 2, 3, 4} otherwise a std::invalid_argument is thrown)
   * */
  Vector4 get_row(gsize i)const throw(std::invalid_argument)
  {
    switch(i)
    {
    case 1:
      return Vector4(m[0],  m[4],  m[8],  m[12]);
    case 2:
      return Vector4(m[1],  m[5],  m[9],  m[13]);
    case 3:
      return Vector4(m[2],  m[6],  m[10], m[14]);
    case 4:
      return Vector4(m[3],  m[7],  m[11], m[15]);
    default:
      throw std::invalid_argument("**Matrix44::get_row** i must be one of {1, 2, 3, 4}");
    }
  }

  /** @name Debugging
   * */
  //@{
  std::string str()const
  {
    return Glib::ustring::compose("%1\n%2\n%3\n%4", get_row(1).str(), get_row(2).str(), get_row(3).str(), get_row(4).str()).c_str();
  }
  //@}

  static const Matrix44 identity;
};

#endif