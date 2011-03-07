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

#include <cairomm/cairomm.h>
#include <gdkmm/pixbuf.h>

Cairo::RefPtr<Cairo::ImageSurface> create_cairo_imagesurface_from_pixbuf(Glib::RefPtr<const Gdk::Pixbuf> pixbuf);

/**
 * \note If the area defined by clip_x, clip_y, clip_width and clip_height is partially or totally outside cairo_img's image area, an exception will be thrown
 *
 * \param cairo_img
 *   \li Format: only Cairo::FORMAT_ARGB32 and Cairo::FORMAT_RGB24 are supported - otherwise an exception will be thrown
 *   \li if empty ptr or empty image area an empty pixbuf refptr will be returned
 * \param clip_width if<0, the while image width-clip_x will be used
 * \param clip_height if<0, the while image height-clip_y will be used
 *
 * \todo renam
 * */
Glib::RefPtr<Gdk::Pixbuf> create_cairo_imagesurface_from_pixbuf(Cairo::RefPtr<const Cairo::ImageSurface> cairo_img, int clip_x=0, int clip_y=0, int clip_width=-1, int clip_height=-1);

void clear_cairo_img(const Cairo::RefPtr<Cairo::ImageSurface>& cairo_img, gdouble r, gdouble g, gdouble b, gdouble a);

/** \brief Remaps  the colors of a cairo image.
 *
 * Uses the luminance of the pixels to interpolate between two colors.
 *
 * Black will be replaced with the color defined with br, bg, bb, and ba.<br>
 * White will be replaced with the color defined with wr, wg, wb, and wa. Any other luminances will be interpolated
 *
 *
 * */
void cairo_img_map_color(const Cairo::RefPtr<Cairo::ImageSurface>& cairo_img, gdouble br, gdouble bg, gdouble bb, gdouble ba, gdouble wr, gdouble wg, gdouble wb, gdouble wa);
