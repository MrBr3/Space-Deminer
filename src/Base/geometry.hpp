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

/** \brief Checks whether a line cuts a vertical line
 *
 * \param a_from sets the beginning of the line
 * \param a_to sets the end of the line
 * \param b_from sets the beginning of the vertical line
 * \param b_height sets the height of the vertical line (negative values mean the line to go to the oppostite direction of the y axis)
 * */
bool line_cuts_vline(const Vector2& a_from, const Vector2& a_to, const Vector2& b_from, gfloat b_height);

/** \brief Checks whether a line cuts a horizontal line
 *
 * \param a_from sets the beginning of the line
 * \param a_to sets the end of the line
 * \param b_from sets the beginning of the horizontal line
 * \param b_width sets the width of the horizontal line (negative values mean the line to go to the oppostite direction of the x axis)
 * \param b_is_a_ray if set, line b starts at b_from and goes to infinity (direction specified, whether b_widthis positive or negative)
 * */
bool line_cuts_hline(const Vector2& a_from, const Vector2& a_to, const Vector2& b_from, gfloat b_width, bool b_is_a_ray);

/** \brief Checks, wether a point is within an ngon.
 *
 * The algorithm check, whether there's a line of the ngon left and right of the point \c p
 *
 * \param p the point to check
 * \param points the points describing the ngon. \c points[0] is thought to be connected to \c points[1], \c points[1] to \c points[2], ... \c points[n_points-1] to \c points[0].
 * \param n_points the number of points stored in the \c points array.
 * */
bool within_ngon(const Vector2& p, const Vector2* points, gsize n_points);

/** \brief Checks, whether \c x is within the interval described with \c a and \c b
*
* Returns the truth value of \f$x\in\left[min\{a,b\},\,max\{a,b\}\right]\f$
*
* The implementation:
* \code
return x>=MIN(a,b) && x<=MAX(a,b);
* \endcode
* */
inline bool between(gfloat x, gfloat a, gfloat b)
{
  return x>=MIN(a,b) && x<=MAX(a,b);
}

/** \brief Checks, whether two intervalls are intersecting.
*
* Returns the truth value of \f$\{\}\;\neq\;\left[min\{a_1,a_2\},\,max\{a_1,a_2\}\right]\;\cap\;\left[min\{b_1,b2\},\,max\{b_1,b_2\}\right]\f$
*
* The implementation:
* \code
return between(a1, b1, b2) ||
       between(a2, b1, b2) ||
       between(b1, a1, a2) ||
       between(b2, a1, a2);
* \endcode
* */
bool intersection_1d(gfloat a1, gfloat a2, gfloat b1, gfloat b2);

#endif