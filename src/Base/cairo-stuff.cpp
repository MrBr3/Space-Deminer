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

#include "./cairo-stuff.hpp"

Cairo::RefPtr<Cairo::ImageSurface> create_cairo_imagesurface_from_pixbuf(Glib::RefPtr<const Gdk::Pixbuf> pixbuf)
{
  if(!pixbuf || pixbuf->get_width()<=0 || pixbuf->get_height()<=0)
    return Cairo::RefPtr<Cairo::ImageSurface>();

  Cairo::Format cairo_format;
  guint8 *cairo_data, *pixbuf_data;
  gsize c_stride, pb_stride;
  gsize width  = pixbuf->get_width();
  gsize height = pixbuf->get_height();

  if(pixbuf->get_has_alpha())
    cairo_format  = Cairo::FORMAT_ARGB32;
  else
    cairo_format  = Cairo::FORMAT_RGB24;

  Cairo::RefPtr<Cairo::ImageSurface> cairo_image = Cairo::ImageSurface::create(cairo_format, width, height);

  g_assert(cairo_image);

  cairo_data  = cairo_image->get_data();
  pixbuf_data = pixbuf->get_pixels();

  c_stride = cairo_image->get_stride();
  pb_stride = pixbuf->get_rowstride();

  g_assert(cairo_data);
  g_assert(pixbuf_data);

  gfloat inv_0xff = 1.f/255.f;

  if(cairo_format==Cairo::FORMAT_ARGB32)
  {
    for(gsize y=0; y<height; ++y)
    for(gsize x=0; x<width; ++x)
    {
      gfloat alpha  = pixbuf_data[y*pb_stride + x*4 + 3]*inv_0xff;
      cairo_data[y*c_stride + x*4 + 0]  = guint8(pixbuf_data[y*pb_stride + x*4 + 2]*alpha);
      cairo_data[y*c_stride + x*4 + 1]  = guint8(pixbuf_data[y*pb_stride + x*4 + 1]*alpha);
      cairo_data[y*c_stride + x*4 + 2]  = guint8(pixbuf_data[y*pb_stride + x*4 + 0]*alpha);
      cairo_data[y*c_stride + x*4 + 3]  = pixbuf_data[y*pb_stride + x*4 + 3];
    }
  }else
  {
    //TODO: TEST, whether dis works, too
    for(gsize y=0; y<height; ++y)
    for(gsize x=0; x<width; ++x)
    {
      cairo_data[y*c_stride + x*4 + 0]  = pixbuf_data[y*pb_stride + x*4 + 2];
      cairo_data[y*c_stride + x*4 + 1]  = pixbuf_data[y*pb_stride + x*4 + 1];
      cairo_data[y*c_stride + x*4 + 2]  = pixbuf_data[y*pb_stride + x*4 + 0];
    }
  }

  cairo_image->mark_dirty();

  return cairo_image;
}

Glib::RefPtr<Gdk::Pixbuf> create_cairo_imagesurface_from_pixbuf(Cairo::RefPtr<const Cairo::ImageSurface> cairo_img, int clip_x, int clip_y, int clip_width, int clip_height)
{
  Glib::RefPtr<Gdk::Pixbuf> pb_img;

  if(!cairo_img || cairo_img->get_width()<=0 || cairo_img->get_height()<=0)
    return pb_img;

  if(clip_width<0)
    clip_width  = cairo_img->get_width()-clip_x;
  if(clip_height<0)
    clip_height  = cairo_img->get_height()-clip_y;

  if(clip_width==0 || clip_height==0)
    return pb_img;

  if(clip_x+clip_width>cairo_img->get_width())
    throw std::invalid_argument("**create_cairo_imagesurface_from_pixbuf** clip_x+clip_width>cairo_img->get_width()\n");

  if(clip_y+clip_height>cairo_img->get_height())
    throw std::invalid_argument("**create_cairo_imagesurface_from_pixbuf** clip_y+clip_height>cairo_img->get_height()\n");

  if(cairo_img->get_format()!=Cairo::FORMAT_ARGB32 && cairo_img->get_format()!=Cairo::FORMAT_RGB24)
    throw std::invalid_argument("**create_cairo_imagesurface_from_pixbuf** cairo_img - only Cairo::FORMAT_ARGB32 and Cairo::FORMAT_RGB24 are supported!\n");

  bool use_alpha  = cairo_img->get_format()==Cairo::FORMAT_ARGB32;

  int clip_x2 = clip_x+clip_width;
  int clip_y2 = clip_y+clip_height;

  pb_img  = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, use_alpha, 8, clip_width, clip_height);

  const gsize dest_row_stride  = pb_img->get_rowstride();
  const gsize src_row_stride  = cairo_img->get_stride();

  guint8* dest_pixel_line = pb_img->get_pixels() + dest_row_stride*clip_y;
  const guint8* src_pixel_line  = (const guint8*)(cairo_img->get_data()) + src_row_stride*clip_y;

  gfloat inv_0xff = 1.f/255.f;

  if(use_alpha)
  {
    for(gsize y=0; y<clip_height; ++y)
    {
      guint8* dest_pixel  = dest_pixel_line + clip_x*4;
      const guint8* src_pixel = src_pixel_line + clip_x*4;

      for(gsize x=0; x<clip_width; ++x)
      {
        gfloat inv_alpha  = 1.f / (inv_0xff * src_pixel[3]);

        dest_pixel[3]  = guint8(src_pixel[3]);
        dest_pixel[0]  = guint8(src_pixel[2]*inv_alpha);
        dest_pixel[1]  = guint8(src_pixel[1]*inv_alpha);
        dest_pixel[2]  = guint8(src_pixel[0]*inv_alpha);

        src_pixel += 4;
        dest_pixel += 4;
      }
      dest_pixel_line += dest_row_stride;
      src_pixel_line += src_row_stride;
    }
  }else
  {
    for(gsize y=0; y<clip_height; ++y)
    {
      guint8* dest_pixel  = dest_pixel_line + clip_x*4;
      const guint8* src_pixel = src_pixel_line + clip_x*4;

      for(gsize x=0; x<clip_width; ++x)
      {
        dest_pixel[0]  = guint8(src_pixel[2]);
        dest_pixel[1]  = guint8(src_pixel[1]);
        dest_pixel[2]  = guint8(src_pixel[0]);

        src_pixel += 4;
        dest_pixel += 4;
      }
      dest_pixel_line += dest_row_stride;
      src_pixel_line += src_row_stride;
    }
  }

  return pb_img;
}

void clear_cairo_img(const Cairo::RefPtr<Cairo::ImageSurface>& cairo_image, gdouble r, gdouble g, gdouble b, gdouble a)
{
  Cairo::Format cairo_format;
  guint8 *cairo_data;
  gsize cairo_stride;

  cairo_format = cairo_image->get_format();
  cairo_data  = cairo_image->get_data();
  cairo_stride = cairo_image->get_stride();
  int width = cairo_image->get_width();
  int height = cairo_image->get_height();

  r *= a;
  g *= a;
  b *= a;

  guint8 b_r  = guint8(r*255.0);
  guint8 b_g  = guint8(g*255.0);
  guint8 b_b  = guint8(b*255.0);
  guint8 b_a  = guint8(a*255.0);

  if(cairo_format==Cairo::FORMAT_ARGB32)
  {
    for(gsize y=0; y<height; ++y)
    for(gsize x=0; x<width; ++x)
    {
      cairo_data[y*cairo_stride + x*4 + 0]  = b_b;
      cairo_data[y*cairo_stride + x*4 + 1]  = b_g;
      cairo_data[y*cairo_stride + x*4 + 2]  = b_r;
      cairo_data[y*cairo_stride + x*4 + 3]  = b_a;
    }
  }else if(cairo_format==Cairo::FORMAT_RGB24)
  {
    //TODO: TEST, whether dis works, too
    for(gsize y=0; y<height; ++y)
    for(gsize x=0; x<width; ++x)
    {
      cairo_data[y*cairo_stride + x*4 + 0]  = b_b;
      cairo_data[y*cairo_stride + x*4 + 1]  = b_g;
      cairo_data[y*cairo_stride + x*4 + 2]  = b_r;
    }
  }else
  {
    g_warning("**clear_cairo_img** unsupported format");
  }
}

void cairo_img_map_color(const Cairo::RefPtr<Cairo::ImageSurface>& cairo_image, gdouble br, gdouble bg, gdouble bb, gdouble ba, gdouble wr, gdouble wg, gdouble wb, gdouble wa)
{
  Cairo::Format cairo_format;
  guint8 *cairo_data;
  gsize cairo_stride;

  cairo_format = cairo_image->get_format();
  cairo_data  = cairo_image->get_data();
  cairo_stride = cairo_image->get_stride();
  int width = cairo_image->get_width();
  int height = cairo_image->get_height();

  const gdouble inv_0xff  = 1./255.;

  if(cairo_format==Cairo::FORMAT_ARGB32)
  {
    for(gsize y=0; y<height; ++y)
    for(gsize x=0; x<width; ++x)
    {
      gdouble l = 0.299 * inv_0xff * gdouble(cairo_data[y*cairo_stride + x*4 + 0]) +
                  0.578 * inv_0xff * gdouble(cairo_data[y*cairo_stride + x*4 + 1]) +
                  0.114 * inv_0xff * gdouble(cairo_data[y*cairo_stride + x*4 + 2]);
      l /= inv_0xff * gdouble(cairo_data[y*cairo_stride + x*4 + 3]);
      gdouble a = ba + (wa-ba)*l;
      gdouble r = (br + (br-br)*l)*a;
      gdouble g = (bg + (bg-bg)*l)*a;
      gdouble b = (bb + (bb-bb)*l)*a;

      cairo_data[y*cairo_stride + x*4 + 0]  = guint8(CLAMP(b*255., 0., 255.));
      cairo_data[y*cairo_stride + x*4 + 1]  = guint8(CLAMP(g*255., 0., 255.));
      cairo_data[y*cairo_stride + x*4 + 2]  = guint8(CLAMP(r*255., 0., 255.));
      cairo_data[y*cairo_stride + x*4 + 3]  = guint8(CLAMP(a*255., 0., 255.));
    }
  }else if(cairo_format==Cairo::FORMAT_RGB24)
  {
    //TODO: TEST, whether dis works, too
    for(gsize y=0; y<height; ++y)
    for(gsize x=0; x<width; ++x)
    {
      gdouble l = gdouble(cairo_data[y*cairo_stride + x*4 + 0] + cairo_data[y*cairo_stride + x*4 + 1] + cairo_data[y*cairo_stride + x*4 + 2]) / (3.0*255.);
      gdouble r = br + (br-br)*l;
      gdouble g = bg + (bg-bg)*l;
      gdouble b = bb + (bb-bb)*l;

      cairo_data[y*cairo_stride + x*4 + 0]  = guint8(CLAMP(b*255., 0., 255.));
      cairo_data[y*cairo_stride + x*4 + 1]  = guint8(CLAMP(g*255., 0., 255.));
      cairo_data[y*cairo_stride + x*4 + 2]  = guint8(CLAMP(r*255., 0., 255.));
    }
  }else
  {
    g_warning("**cairo_img_map_color** unsupported format");
  }
}
