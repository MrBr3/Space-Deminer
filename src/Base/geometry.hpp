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

#ifndef _SPACE_DEMINER_BASE_GEOMETRY_H_
#define _SPACE_DEMINER_BASE_GEOMETRY_H_

inline bool is_in_rectangle(int x, int y, const Gdk::Rectangle& rect)
{
  return x>=rect.get_x() && y>=rect.get_y() && x<rect.get_x()+rect.get_width() && y<rect.get_y()+rect.get_height();
}

/** \brief Gets whether a line cuts a vertical line
 *
 * \param a_from sets the beginning of the line
 * \param a_to sets the end of the line
 * \param b_from sets the beginning of the vertical line
 * \param b_height sets the height of the vertical line (negative values mean the line to go to the oppostite direction of the y axis)
 * */
bool line_cuts_vline(const Vector2& a_from, const Vector2& a_to, const Vector2& b_from, gfloat b_height);

/** \brief Gets whether a line cuts a horizontal line
 *
 * \param a_from sets the beginning of the line
 * \param a_to sets the end of the line
 * \param b_from sets the beginning of the horizontal line
 * \param b_width sets the width of the horizontal line (negative values mean the line to go to the oppostite direction of the x axis)
 * \param b_is_a_ray if set, line b starts at b_from and goes to infinity (direction specified, whether b_widthis positive or negative)
 * */
bool line_cuts_hline(const Vector2& a_from, const Vector2& a_to, const Vector2& b_from, gfloat b_width, bool b_is_a_ray);

bool within_ngon(const Vector2& point, const Vector2* points, gsize n_points);

#endif