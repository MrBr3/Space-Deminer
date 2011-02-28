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

#include <base.hpp>

Matrix44Stack::Matrix44Stack()
{
  _depth = 0;
  _top_matrix = nullptr;

  set_depth(1);
}

Matrix44Stack::~Matrix44Stack()throw()
{
}

void Matrix44Stack::set_depth(gsize d)
{
  _depth = d = MAX(1, d);
  gsize new_capacity = MAX(16, d-(d%16))+8;

  if(new_capacity>_stack.size())
    _stack.resize(new_capacity, DONT_INIT);

  _top_matrix = &_stack[d];
}

void Matrix44Stack::push(bool copy)
{
  set_depth(get_depth()+1);
  const Matrix44& prev_matrix = _stack[get_depth()-1];

  if(copy)
    top() = prev_matrix;
  else
    top().set_identity();
}

void Matrix44Stack::clear()
{
  set_depth(1);
  top().set_identity();
}
