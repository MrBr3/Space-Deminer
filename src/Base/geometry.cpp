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

#include "./base.hpp"

bool between(gfloat x, gfloat a, gfloat b)
{
  return x>=MIN(a,b) && x<=MAX(a,b);
}

bool intersection_1d(gfloat a1, gfloat a2, gfloat b1, gfloat b2)
{
  return between(a1, b1, b2) ||
         between(a2, b1, b2) ||
         between(b1, a1, a2) ||
         between(b2, a1, a2);
}

bool line_cuts_vline(const Vector2& a_from, const Vector2& a_to, const Vector2& b_from, gfloat b_height)
{
  if(!between(b_from.x, a_from.x, a_to.x))
    return false;

  if(a_from.x==a_to.x)
    return a_from.x==b_from.x && intersection_1d(a_from.y, a_to.y, b_from.y, b_from.y+b_height);

  gfloat m  = (a_to.y-a_from.y)/(a_to.x-a_from.x);
  gfloat c = a_from.y-m*a_from.x;

  gfloat cut_y  = m*b_from.x+c;

  return between(cut_y, b_from.y, b_from.y+b_height);
}

bool line_cuts_hline(const Vector2& a_from, const Vector2& a_to, const Vector2& b_from, gfloat b_width, bool b_is_a_ray)
{
  b_is_a_ray &= b_width!=0.f;

  if(!between(b_from.y, a_from.y, a_to.y))
    return false;

  if(a_from.y==a_to.y)
  {
    if(a_from.y!=b_from.y)
      return false;
    if(b_is_a_ray && b_width>0.f)
      return a_from.x>=b_from.x || a_to.x>=b_from.x;
    else if(b_is_a_ray && b_width<0.f)
      return a_from.y==b_from.y && (a_from.x<=b_from.x || a_to.x<=b_from.x);
    else
      return intersection_1d(a_from.y, a_to.y, b_from.y, b_from.y+b_width);
  }

  gfloat cut_x;

  if(a_from.x==a_to.x)
  {
    cut_x = a_from.x;
  }else
  {
    gfloat m  = (a_to.y-a_from.y)/(a_to.x-a_from.x);
    gfloat c = a_from.y-m*a_from.x;

    g_assert(m!=0.f);

    cut_x  = (b_from.y-c)/m;
  }

  if(b_is_a_ray)
  {
    if(b_width>0.f)
      return cut_x>b_from.x;
    else
      return cut_x<b_from.x;
  }

  return between(cut_x, b_from.x, b_from.x+b_width);
}

bool within_ngon(const Vector2& p, const Vector2* points, gsize n_points)
{
  if(!points)
    throw std::invalid_argument("within_ngon points==nullptr");

  bool left=false, right=false;

  for(gsize i=0; i<n_points; ++i)
  {
    const Vector2& curr = points[i];
    const Vector2& next = points[(i+1)%n_points];

    left|=line_cuts_hline(curr, next, p, 1.f, true);
    right|=line_cuts_hline(curr, next, p,-1.f, true);
  }

  return left&&right;
}