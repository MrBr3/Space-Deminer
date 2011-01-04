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

#ifndef _SPACE_DEMINER_VECTOR_CREATORS_H_
#define _SPACE_DEMINER_VECTOR_CREATORS_H_

/** \brief creates an empty std::vector
 * */
template<class T>
std::vector<T> create_vector()
{
  return std::vector<T>();
}

/** \brief creates a std::vector with one element
 * */
template<class T>
std::vector<T> create_vector(const T& t1)
{
  std::vector<T> v;

  v.push_back(t1);

  return v;
}

/** \brief creates a std::vector with two elements
 * */
template<class T>
std::vector<T> create_vector(const T& t1, const T& t2)
{
  std::vector<T> v;

  v.push_back(t1);
  v.push_back(t2);

  return v;
}

/** \brief creates a std::vector with three elements
 * */
template<class T>
std::vector<T> create_vector(const T& t1, const T& t2, const T& t3)
{
  std::vector<T> v;

  v.push_back(t1);
  v.push_back(t2);
  v.push_back(t3);

  return v;
}

/** \brief creates a std::vector with four elements
 * */
template<class T>
std::vector<T> create_vector(const T& t1, const T& t2, const T& t3, const T& t4)
{
  std::vector<T> v;

  v.push_back(t1);
  v.push_back(t2);
  v.push_back(t3);
  v.push_back(t4);

  return v;
}

/** \brief creates a std::vector with five elements
 * */
template<class T>
std::vector<T> create_vector(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5)
{
  std::vector<T> v;

  v.push_back(t1);
  v.push_back(t2);
  v.push_back(t3);
  v.push_back(t4);
  v.push_back(t5);

  return v;
}

/** \brief creates a std::vector with six elements
 * */
template<class T>
std::vector<T> create_vector(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5, const T& t6)
{
  std::vector<T> v;

  v.push_back(t1);
  v.push_back(t2);
  v.push_back(t3);
  v.push_back(t4);
  v.push_back(t5);
  v.push_back(t6);

  return v;
}

/** \brief creates a std::vector with seven elements
 * */
template<class T>
std::vector<T> create_vector(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5, const T& t6, const T& t7)
{
  std::vector<T> v;

  v.push_back(t1);
  v.push_back(t2);
  v.push_back(t3);
  v.push_back(t4);
  v.push_back(t5);
  v.push_back(t6);
  v.push_back(t7);

  return v;
}

/** \brief creates a std::vector with eight elements
 * */
template<class T>
std::vector<T> create_vector(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5, const T& t6, const T& t7, const T& t8)
{
  std::vector<T> v;

  v.push_back(t1);
  v.push_back(t2);
  v.push_back(t3);
  v.push_back(t4);
  v.push_back(t5);
  v.push_back(t6);
  v.push_back(t7);
  v.push_back(t8);

  return v;
}

/** \brief creates a std::vector with nine elements
 * */
template<class T>
std::vector<T> create_vector(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5, const T& t6, const T& t7, const T& t8, const T& t9)
{
  std::vector<T> v;

  v.push_back(t1);
  v.push_back(t2);
  v.push_back(t3);
  v.push_back(t4);
  v.push_back(t5);
  v.push_back(t6);
  v.push_back(t7);
  v.push_back(t8);
  v.push_back(t9);

  return v;
}

/** \brief creates a std::vector with ten elements
 * */
template<class T>
std::vector<T> create_vector(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5, const T& t6, const T& t7, const T& t8, const T& t9, const T& t10)
{
  std::vector<T> v;

  v.push_back(t1);
  v.push_back(t2);
  v.push_back(t3);
  v.push_back(t4);
  v.push_back(t5);
  v.push_back(t6);
  v.push_back(t7);
  v.push_back(t8);
  v.push_back(t9);
  v.push_back(t10);

  return v;
}

#endif