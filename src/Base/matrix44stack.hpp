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

#ifndef _SPACE_DEMINER_MATRIX44STACK_H_
#define _SPACE_DEMINER_MATRIX44STACK_H_

class Matrix44Stack
{
  std::vector<Matrix44> _stack;
  gsize _depth;
  Matrix44* _top_matrix;

  void set_depth(gsize d);

public:
  const Matrix44& top()const{return *_top_matrix;}

  Matrix44& top(){return *_top_matrix;}

  void push(bool copy);
  void pop(){g_assert(_depth>0);set_depth(get_depth()-1);}

  gsize get_depth()const{return _depth;}

public:
  void load_top_to_opengl()const;
  void clear();

  Matrix44Stack();
  ~Matrix44Stack()throw();
};

#endif
